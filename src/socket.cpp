#include "socket.h"

#include "logging.h"

#include <string>
#include <string.h>

namespace {

std::string to_string(struct pbuf* data) {
  std::string buffer;
  buffer.resize(data->tot_len);
  struct pbuf* curr_buff = data;
  uint32_t bytes_copied = 0;
  while(curr_buff) {
    memcpy(&(buffer[bytes_copied]), curr_buff->payload, curr_buff->len);
    bytes_copied += curr_buff->len;
    curr_buff = curr_buff->next;
  }

  return buffer;
}

template <typename T>
err_t tcp_rx(void* arg, struct tcp_pcb* conn, struct pbuf* data, err_t error) {
  if(error != ERR_OK) {
    log_e("Socket RX Error: {}", error);
    ((T*)(arg))->error(error);
    return ERR_OK;
  }
  if(data == NULL) { return ERR_OK; }

  auto buffered_data = to_string(data);
  pbuf_free(data);
  ((T*)(arg))->read(buffered_data);
  log_d("Socket RX: {}", buffered_data.size());
  return ERR_OK;
}

template <typename T>
err_t tcp_acc(void* arg, struct tcp_pcb* conn, err_t error) {
  if(error != ERR_OK) {
    log_e("Socket Accept Error: {}", error);
    return ERR_OK;
  }
  ((T*)(arg))->accept(conn);
  log_d("Socket Accept");
  return ERR_OK;
}

template <typename T>
void tcp_err(void* arg, err_t error) {
  log_e("Socket Error: {}", error);
  ((T*)(arg))->error(error);
}

template <typename T>
err_t tcp_tx(void* arg, struct tcp_pcb* conn, uint16_t len) {
  ((T*)(arg))->sent(len);
  return ERR_OK;
}


} // namespace



bool Socket::listen(uint32_t port) {
  _mode = SocketMode::listen;
  _state = SocketState::disconnected;
  _port = port;

  // Initialize all the lwip structs
  _tcp_handle = tcp_new();
  if(_tcp_handle == NULL) { return false; }

  if(tcp_bind(_tcp_handle, IP_ANY_TYPE, _port) != ERR_OK) {
    return false;
  }

  tcp_arg(_tcp_handle, this);

  _tcp_handle = tcp_listen_with_backlog(_tcp_handle, 10);
  if(_tcp_handle == NULL) {
    return false;
  }

  tcp_accept(_tcp_handle, tcp_acc<Socket>);

  return true;
}

void Socket::shutdown() {
  _close_after_send = true;
}

void Socket::read(std::string& data) {
  if(_delegate == NULL) { return; }
  _delegate->handle_rx(this, data);
}

void Socket::sent(uint16_t sent_len) {
  if(sent_len > 0 && _close_after_send) {
    close();
  }
};

uint32_t Socket::write(const uint8_t* buffer, uint32_t size) {
  if(_mode == SocketMode::listen) { return 0; }
  if(_state != SocketState::connected) { return 0; }
  err_t error = tcp_write(_tcp_handle, buffer, size, TCP_WRITE_FLAG_COPY);
  if(error != ERR_OK) { return 0; }
  return size;
}

void Socket::accept(struct tcp_pcb* conn) {
  if(_delegate == NULL) { return; }
  Socket* new_conn = new Socket(conn, _port);
  _delegate->handle_accept(new_conn);
}

void Socket::error(err_t error) {
  _tcp_handle = NULL;  /// @note LWIP will have deleted this handle already for us on error state
  _state = SocketState::disconnected;
  if(_delegate == NULL) { return; }
  _delegate->handle_closed(this);
}

Socket::Socket(struct tcp_pcb* conn, uint32_t port) {
  _port = port;
  _tcp_handle = conn;
  _mode = SocketMode::connection;
  _state = SocketState::connected;
  tcp_arg(conn, this);
  tcp_recv(conn, tcp_rx<Socket>);
  tcp_sent(conn, tcp_tx<Socket>);
  tcp_err(conn, tcp_err<Socket>);
}

void Socket::close() {
  if(_mode == SocketMode::connection && _state == SocketState::connected) {
    tcp_shutdown(_tcp_handle, 1, 1);
    _state = SocketState::disconnected;
    if(_delegate != NULL) {
      _delegate->handle_closed(this);
    }
  }
}

void Socket::flush() {
  tcp_output(_tcp_handle);
}
