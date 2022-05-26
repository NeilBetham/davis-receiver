#include "logging_uart.h"

void LoggingUart::submit_log(const std::string& message) {
  _uart.send(message.c_str());
}
