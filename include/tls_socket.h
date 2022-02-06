/**
 * @brief Manages a TLS socket
 */

#pragma once

#include "socket_delegate.h"
#include "socket.h"

class TLSSocket : public SocketDelegate {
public:
  TLSSocket(Socket& socket);
  ~TLSSocket() {};

  void set_delegate(SocketDelegate* delegate) { _delegate = delegate; }
  bool connected();

  // SocketDelegate callbacks
  void handle_rx(Socket* conn, std::string& data);
  void handle_closed(Socket* conn);
  void handle_tx(Socket* conn);

  // mbedtls hooks
  int recv(uint8_t* buffer, uint32_t len);
  int send(uint8_t* buffer, uint32_t len);

private:
  Socket& _socket; // Not totally sure on the inheritance vs composition here yet...
  SocketDelegate* _delegate = NULL;

  mbedtls_entropy_context _entropy;
  mbedtls_ctr_drbg_context _ctr_drbg;
  mbedtls_ssl_context _ssl;
  mbedtls_ssl_config _conf;
  mbedtls_x509_crt _cacert;
};
