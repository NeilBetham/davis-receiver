#include "packet_parser.h"

#include <string>
#include <string.h>

static const std::string unknown = "Unknown";
static const std::string super_cap_voltage = "SuperCapVoltage";
static const std::string uv_index = "UVIndex";
static const std::string rain_rate = "RainRate";
static const std::string solar_radiation = "SolarRadiation";
static const std::string light = "Light";
static const std::string temperature = "Temperature";
static const std::string wind_speed_gusts = "WindSpeedGusts";
static const std::string humidity = "Humidity";
static const std::string rain_clicks = "RainClicks";
static const std::string wind_speed = "WindSpeed";
static const std::string wind_dir = "WindDir";

const std::string& reading_type_string(ReadingType reading_type) {
  switch(reading_type) {
    case ReadingType::super_cap_voltage: return super_cap_voltage;
    case ReadingType::uv_index: return uv_index;
    case ReadingType::rain_rate: return rain_rate;
    case ReadingType::solar_radiation: return solar_radiation;
    case ReadingType::light: return light;
    case ReadingType::temperature: return temperature;
    case ReadingType::wind_speed_gusts: return wind_speed_gusts;
    case ReadingType::humidity: return humidity;
    case ReadingType::rain_clicks: return rain_clicks;
    case ReadingType::wind_speed: return wind_speed;
    case ReadingType::wind_dir: return wind_dir;
    default: return unknown;
  }
}

float convert_super_cap_voltage(const Reading& reading) {
  return ((float)(((uint32_t)reading.raw_data[0]) << 2) + ((float)((uint32_t)reading.raw_data[1] & 0xC0) / 64)) / 100;
}

float convert_uv_index(const Reading& reading) {
  return 0.0;
}

float convert_rain_rate(const Reading& reading) {
  // TODO: Verify this is correct, I very seriously doubt that it is, or I implemented it wrong which is also very possible
  if(reading.raw_data[0] == 0xFF) {
    return 0.0;
  } else if(reading.raw_data[1] & 0x40 == 0) {
    return ((float)(reading.raw_data[1] & 0x30) / 16 * 250) + (float)(reading.raw_data[0]);
  } else if(reading.raw_data[1] & 0x40 == 0x40) {
    return ((float)(reading.raw_data[1] & 0x30) / 16 * 250) + ((float)(reading.raw_data[0]) / 16);
  }
  return 0.0;
}

float convert_solar_radiation(const Reading& reading) {
  return 0.0;
}

float convert_light(const Reading& reading) {
  return ((float)reading.raw_data[0] * 4) + ((float)(reading.raw_data[1] & 0xC0) / 64);
}

float convert_temperature(const Reading& reading) {
  return (float)(((uint32_t)reading.raw_data[0] << 8) + (uint32_t)reading.raw_data[1]) / 160;
}

float convert_wind_speed_gusts(const Reading& reading) {
  return (float)reading.raw_data[0];
}

float convert_humidity(const Reading& reading) {
  return (float)((((uint32_t)reading.raw_data[1] & 0xF0) << 4) + ((uint32_t)reading.raw_data[0])) / 10;
}

float convert_rain_clicks(const Reading& reading) {
  return (float)(reading.raw_data[0] & 0x7F);
}

static float convert_reading_to_float(const Reading& reading) {
  switch(reading.type) {
    case ReadingType::super_cap_voltage: return convert_super_cap_voltage(reading);
    case ReadingType::uv_index: return convert_uv_index(reading);
    case ReadingType::rain_rate: return convert_rain_rate(reading);
    case ReadingType::solar_radiation: return convert_solar_radiation(reading);
    case ReadingType::light: return convert_light(reading);
    case ReadingType::temperature: return convert_temperature(reading);
    case ReadingType::wind_speed_gusts: return convert_wind_speed_gusts(reading);
    case ReadingType::humidity: return convert_humidity(reading);
    case ReadingType::rain_clicks: return convert_rain_clicks(reading);
    default: return 0.0;
  }
}


Reading parse_packet(ReceivedPacket received_packet) {
  Reading ret;
	ret.packet = received_packet;

  ret.wind_speed = ret.packet.data[1];
  ret.wind_dir = (((uint16_t)ret.packet.data[2]) * 360) / 255;
	memcpy(ret.raw_data, &ret.packet.data[3], 3);
	ret.type = (ReadingType)ret.packet.sensor_id;
	ret.value =  convert_reading_to_float(ret);

  return ret;
}
