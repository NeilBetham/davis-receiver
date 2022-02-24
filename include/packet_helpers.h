/**
 * @brief Helpers for converting packets to JSON for posting to a server
 */

#pragma once


#include "packet_parser.h"

#include <string>


std::string value_json_payload(const Reading& packet);
std::string wind_speed_json_payload(const Reading& packet);
std::string wind_dir_payload(const Reading& packet);
std::string root_reading_payload(const std::string& reading_sub_payload);
