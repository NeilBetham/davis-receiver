/**
 * @brief Implements the logging sink interface for a local uart
 */

#pragma once

#include "logging_sink.h"
#include "uart.h"

class LoggingUart : public LoggingSink {
public:
  LoggingUart(UART& uart) : _uart(uart) {};

  void submit_log(const std::string& message);

private:
  UART& _uart;
};
