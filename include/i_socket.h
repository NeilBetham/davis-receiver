/**
 * @brief Socket interface
 */

#pragma once

#include <stdint.h>

class SocketDelegate;

class ISocket {
public:
  virtual ~ISocket() {};

  virtual uint32_t write(const uint8_t* buffer, uint32_t size) = 0;
  virtual void flush() = 0;
  virtual void shutdown() = 0;
  virtual bool is_connected() = 0;
  virtual void set_delegate(SocketDelegate* delegate) = 0;
};
