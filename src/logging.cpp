#include "logging.h"

#include <list>

static LogLevel global_log_level = LogLevel::unknown;
static std::list<LoggingSink*> global_logging_sinks;


void logging_set_log_level(LogLevel level) {
  global_log_level = level;
}

void logging_add_sink(LoggingSink* sink) {
  global_logging_sinks.push_back(sink);
}

void logging_remove_sink(LoggingSink* sink) {
  for(auto sink_it = global_logging_sinks.begin(); sink_it != global_logging_sinks.end(); sink_it++) {
    if(*sink_it == sink) {
      global_logging_sinks.erase(sink_it);
    }
  }
}

void log_message(LogLevel level, const std::string& message) {
  if(level == LogLevel::unknown) { return; }

  // Check if the log level should be printed
  if(level < global_log_level) { return; }

  // Write to available sinks
  for(auto sink_ptr : global_logging_sinks) {
    sink_ptr->submit_log(message);
  }
}

