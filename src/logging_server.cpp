#include "logging_server.h"

#include <string.h>

void LoggingServer::start() {
  _socket.set_delegate(this);
  _socket.listen(_port);
}

void LoggingServer::stop() {
  for(auto conn_ptr : _active_conns) {
    conn_ptr->shutdown();
  }
  _socket.shutdown();
}

void LoggingServer::submit_log(const std::string& message) {
  for(auto conn_ptr : _active_conns) {
    conn_ptr->write((uint8_t*)message.data(), message.size());
    conn_ptr->flush();
  }

  // Store the log in the log buffer
  uint32_t bytes_till_wrap = sizeof(_log_buffer) - _buffer_write_pos;
  if(bytes_till_wrap > message.size()) {
    // We don't need to worry about wrapping
    memcpy(&_log_buffer[_buffer_write_pos], message.data(), message.size());
    _buffer_write_pos += message.size();
  } else {
    uint32_t second_write_count = message.size() - bytes_till_wrap;
    memcpy(&_log_buffer[_buffer_write_pos], message.data(), bytes_till_wrap);
    memcpy(&_log_buffer[0], message.data() + bytes_till_wrap, second_write_count);
    _buffer_write_pos += message.size();
    _buffer_write_pos %= sizeof(_log_buffer);
  }
}

void LoggingServer::handle_rx(ISocket* conn, const std::string& message) {
  // We don't care about incomming data so just ignore it
  return;
}

void LoggingServer::handle_accept(ISocket* conn) {
  _active_conns.push_back(conn);
  conn->set_delegate(this);

  // Dump the log buffer to the client
  uint32_t first_write_char_count = sizeof(_log_buffer) - _buffer_write_pos;
  conn->write(&_log_buffer[_buffer_write_pos], first_write_char_count);
  conn->write(&_log_buffer[0], _buffer_write_pos);
}

void LoggingServer::handle_closed(ISocket* conn) {
  for(auto sock_it = _active_conns.begin(); sock_it != _active_conns.end(); sock_it++) {
    if(*sock_it == conn) {
      delete *sock_it;
      _active_conns.erase(sock_it);
      break;
    }
  }
}
