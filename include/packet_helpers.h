/**
 * @brief Helpers for converting packets to JSON for posting to a server
 */

#pragma once


#include "received_packet.h"

#include <string>


std::string convert_json_payload(const ReceivedPacket& packet);
