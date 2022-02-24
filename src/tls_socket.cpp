#include "tls_socket.h"

#include "logging.h"
#include "entropy_pool.h"

#include "mbedtls/debug.h"

namespace {


template <typename T>
int recv_shim(void* ctx, uint8_t* buffer, size_t len) {
  return ((T*)(ctx))->recv(buffer, len);
}

template <typename T>
int send_shim(void* ctx, const uint8_t* buffer, size_t len) {
  return ((T*)(ctx))->send(buffer, len);
}

int entropy_source(void* data, uint8_t* output, size_t len, size_t* outlen) {
  return EntropyPool::instance().get_entropy(output, len, outlen);
}

void mbed_debug(void *ctx, int level, const char* file, int line, const char* str) {
  uint32_t len = strlen(str);
  ((char*)str)[len - 1] = 0; // TODO: =P This removes the newline on the mbed dbg msgs
  log_d("MBEDTLS [{}]-{}:{} - {}", level, file, line, str);
}


} // namespace


TLSSocket::TLSSocket(Socket& socket) : _socket(socket) {
  _socket.set_delegate(this);
}

bool TLSSocket::connect(uint32_t ip, uint32_t port) {
  init_tls();
  return _socket.connect(ip, port);
}

bool TLSSocket::connect(const std::string& hostname, uint32_t port) {
  init_tls();
  return _socket.connect(hostname, port);
}

uint32_t TLSSocket::write(const uint8_t* buffer, uint32_t size) {
  int ret = mbedtls_ssl_write(&_ssl, buffer, size);
  if(ret > 0) { return ret; }
  return 0;
}

void TLSSocket::handle_rx(ISocket* conn, const std::string& data) {
  log_d("TLS Handle RX: {}", data.size());
  // Append the resceived data to our internal buffer;
  _buffer += data;

  if(_tls_state == TLSState::init) {
    // Try to compelete the handshale
    uint32_t ret = mbedtls_ssl_handshake(&_ssl);
    if(ret == 0) {
      log_i("TLS Handshake complete");
      _tls_state = TLSState::connected;
      if(_delegate != NULL) {
        _delegate->handle_tx(this);
      }
    } else if(ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
      log_e("TLS Handshake failed");
      _tls_state = TLSState::error;
      mbedtls_ssl_session_reset(&_ssl);
    } else {
      log_d("Handshake incomplete");
    }
  } else if(_tls_state == TLSState::connected) {
    bool reading = true;
    while(reading) {
      std::string buffer;
      buffer.resize(1500);  // TODO: Match the underlying transport MTU dynamically
      int32_t read_bytes = mbedtls_ssl_read(&_ssl, (uint8_t*)&buffer[0], buffer.capacity());

      if(read_bytes == 0) {
        mbedtls_ssl_session_reset(&_ssl);
        _tls_state = TLSState::disconnected;
        reading = false;
      } else if(read_bytes > 0) {
        buffer.resize(read_bytes);
        if(_delegate != NULL) {
          _delegate->handle_rx(this, buffer);
        }
      } else if(read_bytes == MBEDTLS_ERR_SSL_WANT_READ || read_bytes == MBEDTLS_ERR_SSL_WANT_WRITE) {
        reading = false;
      }
    }
  }
  _socket.flush();
}

void TLSSocket::handle_closed(ISocket* conn) {
  log_i("TLS Socket Closed");
  mbedtls_ssl_session_reset(&_ssl);
  _tls_state = TLSState::disconnected;
}

void TLSSocket::handle_tx(ISocket* conn) {
  log_d("TLS Handle TX");
  // If we are in an unknown state that means we need to start a handshake
  if(_tls_state == TLSState::unknown) {
    log_d("Starting TLS Handshake");
    _tls_state = TLSState::init;
    uint32_t ret = mbedtls_ssl_handshake(&_ssl);
    if(ret != 0 && ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
      _tls_state = TLSState::error;
      log_e("Error initializing TLS handshake");
      mbedtls_ssl_session_reset(&_ssl);
    }
  }
}

int TLSSocket::recv(uint8_t* buffer, uint32_t len) {
  if(_buffer.size() < 1) { return MBEDTLS_ERR_SSL_WANT_READ; }

  uint32_t copy_length = std::min(len, (uint32_t)_buffer.size());
  memcpy(buffer, _buffer.data(), copy_length);
  _buffer = _buffer.substr(copy_length, _buffer.size() - copy_length);
  log_d("TLS read: {} -> {}  ({})", len, copy_length, _buffer.size());
  return copy_length;
}

int TLSSocket::send(const uint8_t* buffer, uint32_t len) {
  int bytes_written = _socket.write(buffer, len);
  log_d("TLS write: {} -> {}", len, bytes_written);
  return bytes_written;
}

void TLSSocket::init_tls() {
  mbedtls_ssl_init(&_ssl);
  mbedtls_ssl_config_init(&_conf);
  mbedtls_x509_crt_init(&_cacert);
  mbedtls_ctr_drbg_init(&_ctr_drbg);
  mbedtls_entropy_init(&_entropy);
  mbedtls_entropy_add_source(&_entropy, entropy_source, NULL, 0, MBEDTLS_ENTROPY_SOURCE_STRONG);

  // Seed the number generator
  if(mbedtls_ctr_drbg_seed(&_ctr_drbg, mbedtls_entropy_func, &_entropy, NULL, 0) != 0) {
    log_e("Failed to seed mbedtls PRNG");
  }

  // Setup ssl config
  uint32_t ret = mbedtls_ssl_config_defaults(&_conf,
    MBEDTLS_SSL_IS_CLIENT,
    MBEDTLS_SSL_TRANSPORT_STREAM,
    MBEDTLS_SSL_PRESET_DEFAULT
  );
  if(ret != 0 ) {
    _tls_state = TLSState::error;
    log_e("Failed to init default SSL config");
    return;
  }

  mbedtls_ssl_conf_authmode(&_conf, MBEDTLS_SSL_VERIFY_NONE);
  mbedtls_ssl_conf_ca_chain(&_conf, &_cacert, NULL);
  mbedtls_ssl_conf_rng(&_conf, mbedtls_ctr_drbg_random, &_ctr_drbg);
  mbedtls_ssl_conf_dbg(&_conf, mbed_debug, NULL);
  mbedtls_debug_set_threshold(1);

  // Configure the ssl instance
  ret = mbedtls_ssl_setup(&_ssl, &_conf);
  if(ret != 0) {
    _tls_state = TLSState::error;
    log_e("Failed to setup ssl instance with config");
    return;
  }

  // Configure the network hooks
  mbedtls_ssl_set_bio(&_ssl, this, send_shim<TLSSocket>, recv_shim<TLSSocket>, NULL);
}
