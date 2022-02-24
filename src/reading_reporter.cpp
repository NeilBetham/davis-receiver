#include "config.h"
#include "logging.h"
#include "reading_reporter.h"

#define FMT_HEADER_ONLY
#include <fmt/core.h>


namespace {


static const std::string reading_json_template = "{{\"reading\":{{\
  \"type\": \"{}\",\
  \"raw_value\": \"{:X}{:X}{:X}\",\
  \"decoded_value\": \"{}\"\
}}}}";

static const std::string http_post_template = "POST {} HTTP/1.1 \r\n\
HOST: {} \r\n\
Content-Type: application/json \r\n\
Content-Length: {} \r\n\
Authorization: {} \r\n\
\r\n\
{}";

std::string value_json_payload(const ReadingReport& reading) {
  return fmt::format(
    reading_json_template,
    reading_type_string(reading.type),
    reading.raw_data[0],
    reading.raw_data[1],
    reading.raw_data[2],
    reading.value
  );
}

std::string generate_post_req(const ReadingReport& report) {
	std::string payload = value_json_payload(report);

	return fmt::format(
		http_post_template,
		REPORTING_SERVER_URL,
		REPORTING_SERVER_HOSTNAME,
		payload.size(),
		REPORTING_SERVER_TOKEN,
		payload
	);
}


} // namespace


void ReadingReporter::init() {
  _tls_socket.set_delegate(this);
  connect_to_server();
}

void ReadingReporter::handle_reading(const Reading& reading) {
  connect_to_server();

  // Generate a report for the three elements of the reading
	ReadingReport value_report;
	ReadingReport wind_speed_report;
	ReadingReport wind_dir_report;

	value_report.type = reading.type;
	value_report.value = reading.value;
  memcpy(value_report.raw_data, reading.raw_data, 3);

	wind_speed_report.type = ReadingType::wind_speed;
	wind_speed_report.value = (float)reading.wind_speed;
	wind_speed_report.raw_data[0] = reading.wind_speed;

	wind_dir_report.type = ReadingType::wind_dir;
	wind_dir_report.value = (float)reading.wind_dir;
  memcpy(wind_dir_report.raw_data, (uint8_t*)&reading.wind_dir, 2);

  if(_reading_buffer.can_push()) {
    _reading_buffer.push(value_report);
  } else {
    log_w("Reading dropped, reading buffer full, check connection to server?");
    return;
  }

  if(_reading_buffer.can_push()) {
    _reading_buffer.push(wind_speed_report);
  } else {
    log_w("Reading dropped, reading buffer full, check connection to server?");
    return;
  }

  if(_reading_buffer.can_push()) {
    _reading_buffer.push(wind_dir_report);
  } else {
    log_w("Reading dropped, reading buffer full, check connection to server?");
    return;
  }

  // Start the process to post to the server
  post_reading();
}

void ReadingReporter::handle_rx(ISocket* conn, const std::string& data) {
  // Check the response for the return code
  auto pos = data.find(" ");
  auto return_code = data.substr(pos + 1, 3);
  uint32_t code = std::atoi(return_code.c_str());
  log_d("Return code `{}` => {}", return_code, code);

  if(code < 300) {
    reading_posted(true);
  } else {
    reading_posted(false);
  }
}

void ReadingReporter::handle_closed(ISocket* conn) {
  log_w("Reading reporter could not connect to server");
  // Try to keep the connection open at all times
  // TODO: Add a exponential backoff here
  _socket.connect(REPORTING_SERVER_HOSTNAME, REPORTING_SERVER_PORT);
}

void ReadingReporter::post_reading() {
  // Check if we need to pop a new reading to post
  if(_in_transit_reading_valid == false) {
    if(!_reading_buffer.can_pop()) { return; }
    _in_transit_reading = _reading_buffer.pop();
    _in_transit_reading_valid = true;
  }

  // Check if the socket is connected
  if(!_tls_socket.connected()) { return; }

  // Generate the post request and send it
  std::string reading_post_request = generate_post_req(_in_transit_reading);
  _tls_socket.write((const uint8_t*)reading_post_request.data(), reading_post_request.size());
}

void ReadingReporter::reading_posted(bool successful) {
  if(successful) {
    _in_transit_reading_valid = false;
    if(_reading_buffer.can_pop()) {
      post_reading();
    }
  } else {
    post_reading();
  }
}

void ReadingReporter::connect_to_server() {
  if(!_socket.is_connected()) {
    log_d("Reading Reporter Connecting: {}:{}", REPORTING_SERVER_HOSTNAME, REPORTING_SERVER_PORT);
    _tls_socket.connect(REPORTING_SERVER_HOSTNAME, REPORTING_SERVER_PORT);
  }
}
