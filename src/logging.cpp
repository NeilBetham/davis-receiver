#include "logging.h"

static UART* global_logger_uart = NULL;
static LogLevel global_log_level = LogLevel::unknown;


void logging_init(UART* output_uart) {
  global_logger_uart = output_uart;
}

void logging_set_log_level(LogLevel level) {
  global_log_level = level;
}

void log_message(LogLevel level, const std::string& message) {
  if(level == LogLevel::unknown) { return; }

  // Check if we have an ouput UART to log to
  if(global_logger_uart == NULL) { return; }

  // Check if the log level should be printed
  if(level < global_log_level) { return; }

  // Write to the UART
  global_logger_uart->send(message.c_str());
}

