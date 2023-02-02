/**
 * @brief Provides a tcp socket that outputs logging statements
 */

#pragma once

#include "logging_sink.h"
#include "socket.h"
#include "socket_delegate.h"

#include <list>
#include <string>


class LoggingServer : public SocketDelegate, public LoggingSink {
public:
  LoggingServer(uint32_t port) : _port(port) {};
  ~LoggingServer() {};

  void start();
  void stop();

  void submit_log(const std::string& message);

  // Socket delegate hooks
  void handle_rx(ISocket* conn, const std::string& data);
  void handle_accept(ISocket* conn);
  void handle_closed(ISocket* conn);

private:
  uint32_t _port = 0;
  Socket _socket;
  std::list<ISocket*> _active_conns;
  uint8_t _log_buffer[1024] = {0};
  uint32_t _buffer_write_pos = 0;
};
