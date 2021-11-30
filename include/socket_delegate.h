/**
 * @brief Delegate class to handle Socket connection events
 */

#pragma once

#include <string>

/// @note Clients are responsible for Socket deletion
class Socket;

class SocketDelegate {
public:
  virtual ~SocketDelegate() {};

  virtual void handle_rx(Socket* conn, std::string& data) = 0;
  virtual void handle_closed(Socket* conn) = 0;
  virtual void handle_accept(Socket* conn) = 0;
};
