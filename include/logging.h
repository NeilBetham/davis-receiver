/**
 * @brief Simple logging to UART
 */

#pragma once

#include "uart.h"

#include "timing.h"

#define FMT_HEADER_ONLY
#include <fmt/core.h>
#include <string>

enum class LogLevel : uint8_t {
  unknown = 0,
  debug,
  info,
  warn,
  error
};

void logging_init(UART* output_uart);
void logging_set_log_level(LogLevel level);

void log_message(LogLevel level, const std::string& message);

template <typename ... Ts>
std::string format_args(Ts ... args) {
  return fmt::format(args...);
}


template <typename ... Ts>
void log_e(Ts ... args) {
  auto message = format_args(args...);
  auto msg_hdr = fmt::format("[{:10}][ERROR] - {}\r\n", sys_now(), message);
  log_message(LogLevel::error, msg_hdr);
}

template <typename ... Ts>
void log_w(Ts ... args) {
  auto message = format_args(args...);
  auto msg_hdr = fmt::format("[{:10}][WARN] - {}\r\n", sys_now(), message);
  log_message(LogLevel::warn, msg_hdr);
}

template <typename ... Ts>
void log_i(Ts ... args) {
  auto message = format_args(args...);
  auto msg_hdr = fmt::format("[{:10}][INFO] - {}\r\n", sys_now(), message);
  log_message(LogLevel::info, msg_hdr);
}

template <typename ... Ts>
void log_d(Ts ... args) {
  auto message = format_args(args...);
  auto msg_hdr = fmt::format("[{:10}][DEBUG] - {}\r\n", sys_now(), message);
  log_message(LogLevel::debug, msg_hdr);
}

