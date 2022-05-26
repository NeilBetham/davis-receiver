#include "logging_server.h"

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
}

void LoggingServer::handle_rx(ISocket* conn, const std::string& message) {
  // We don't care about incomming data so just ignore it
  return;
}

void LoggingServer::handle_accept(ISocket* conn) {
  _active_conns.push_back(conn);
  conn->set_delegate(this);
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
