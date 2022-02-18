/**
 * @brief Delegate class to handle Socket connection events
 */

#pragma once

#include <string>

#include "i_socket.h"

/// @note Clients are responsible for Socket deletion
class Socket;

class SocketDelegate {
public:
  virtual ~SocketDelegate() {};

  virtual void handle_rx(ISocket* conn, std::string& data) = 0;
  virtual void handle_closed(ISocket* conn) = 0;

  virtual void handle_accept(ISocket* conn) {};
  virtual void handle_tx(ISocket* conn) {};
};
