/**
 * @brief Generic interface for log messages
 */

#pragma once

#include <string>

class LoggingSink {
public:
  virtual ~LoggingSink() {};

  virtual void submit_log(const std::string& message) = 0;
};
