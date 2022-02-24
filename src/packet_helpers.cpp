#include "packet_helpers.h"

#define FMT_HEADER_ONLY
#include <fmt/core.h>

static const std::string reading_json_template = "{{\
  'type': '{}',\
  'raw_value': '{:X}',\
  'decoded_value': '{}',\
}}";

static const std::string root_reading_json_template = "{{\
  'reading': {}\
}}";

std::string value_json_payload(const Reading& reading) {
  return fmt::format(
    reading_json_template,
    reading_type_string(reading.type),
    reading.raw_data,
    reading.value
  );
}

std::string wind_speed_json_payload(const Reading& reading) {
  return fmt::format(
    reading_json_template,
    "WindSpeed",
    "",
    reading.wind_speed
  );
}

std::string wind_dir_json_payload(const Reading& reading) {
  return fmt::format(
    reading_json_template,
    "WindDir",
    "",
    reading.wind_dir
  );
}

std::string root_reading_payload(const std::string& reading_sub_payload) {
  return fmt::format(root_reading_json_template, reading_sub_payload);
}
