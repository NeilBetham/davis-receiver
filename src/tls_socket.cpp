#include "tls_socket.h"

#include "logging.h"

namespace {


template <typename T>
int recv_shim(void* ctx, uint8_t* buffer, size_t len) {
  return (T*)(ctx)->recv(buffer, len);
}

template <typename T>
int send_shim(void* ctx, uint8_t* buffer, size_t len) {
  return (T*)(ctx)->send(buffer, len);
}


} // namespace


TLSSocket::TLSSocket(Socket& socket) : _socket(socket) {
  _socket.set_delegate(this);

  // Init all the stuff we need for the TLS connection
  mbedtls_ssl_init(&_ssl);
  mbedtls_ssl_confif_init(&_conf);
  mbedtls_x509_crt_init(&_cacert);
  mbedtls_ctr_drbg_init(&_ctr_drbg);

  // Seed data
  // TODO: Generate some entropy from the lower bits of the internal temp ADC
  const char* seed_string = "hackme!";

  // Seed the number generator
  if(mbedtls_ctr_drbg_seed(&_ctr_drbg, mbedtls_entropy_func, &_entropy, (uint8_t*)seed_string, sizeof(seed_string) / sizeof(char)) != 0) {
    log_e("Failed to seed mbedtls PRNG");
  }

  // Configure the network hooks
  mbedtls_ssl_set_bio(&_ssl, this, send_shim<TLSSocket>, recv_shim<TLSSocket>, NULL);

  //
}

bool TLSSocket::connected() {

};

void TLSSocket::handle_rx(Socket* conn, std::string& data) {

}

void TLSSocket::handle_closed(Socket* conn) {

}

void TLSSocket::handle_tx(Socket* conn) {

}

void TLSSocket::recv(uint8_t* buffer, uint32_t len) {



}

void TLSSocket::send(uint8_t* buffer, uint32_t len) {
  socket.write(buffer, len);
}
