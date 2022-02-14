/**
 * @brief Manages a TLS socket
 */

#pragma once

#include "socket_delegate.h"
#include "socket.h"

#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ssl.h>

#include <string>

enum class TLSState {
  unknown = 0,
  init,
  connected,
  error,
  disconnected
};

class TLSSocket : public SocketDelegate {
public:
  TLSSocket(Socket& socket);
  ~TLSSocket() {};

  void set_delegate(SocketDelegate* delegate) { _delegate = delegate; }
  bool connected() { return _tls_state == TLSState::connected; }

  // SocketDelegate callbacks
  void handle_rx(Socket* conn, std::string& data);
  void handle_closed(Socket* conn);
  void handle_tx(Socket* conn);

  // mbedtls hooks
  int recv(uint8_t* buffer, uint32_t len);
  int send(const uint8_t* buffer, uint32_t len);

private:
  Socket& _socket; // Not totally sure on the inheritance vs composition here yet...
  SocketDelegate* _delegate = NULL;
  TLSState _tls_state = TLSState::unknown;
  std::string _buffer;

  mbedtls_entropy_context _entropy;
  mbedtls_ctr_drbg_context _ctr_drbg;
  mbedtls_ssl_context _ssl;
  mbedtls_ssl_config _conf;
  mbedtls_x509_crt _cacert;
};
