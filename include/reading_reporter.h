/**
 * @brief Handles posting packets to a server
 */

#pragma once

#include "packet_parser.h"
#include "socket.h"
#include "tls_socket.h"
#include "socket_delegate.h"
#include "circular_buffer.h"


struct ReadingReport {
  ReadingType type = ReadingType::unknown;
  float value = 0.0;
  uint8_t raw_data[4] = {0};
};


class ReadingReporter : public SocketDelegate {
public:
  ReadingReporter() : _tls_socket(_socket) {};
  ~ReadingReporter() {};

  void init();
  void handle_reading(const Reading& reading);
  void handle_report(const ReadingReport& report);

  // SocketDelegate
  void handle_tx(ISocket* conn) {};
  void handle_rx(ISocket* conn, const std::string& data);
  void handle_closed(ISocket* conn);

  bool connected_to_server() { return _tls_socket.connected(); };

private:
  Socket _socket;
  TLSSocket _tls_socket;
  CircularBuffer<ReadingReport, 60> _reading_buffer;
  ReadingReport _in_transit_reading;
  std::string _rx_buffer;
  bool _in_transit_reading_valid = false;

  void post_reading();
  void reading_posted(bool successful);
  void connect_to_server();
  void handle_response(uint32_t end_pos);
};
