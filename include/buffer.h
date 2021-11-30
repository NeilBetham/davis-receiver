/**
 * @brief Static memory buffer
 */

#pragma once

#include "registers/register_helpers.h"

#include <stdint.h>
#include <string.h>

template <size_t BUFFER_SIZE>
class Buffer {
public:
  Buffer() { zero(); };
  Buffer(Buffer<BUFFER_SIZE> const& other) {
    memcpy(_buffer, other._buffer, BUFFER_SIZE);
    _size = other._size;
  }
  Buffer<BUFFER_SIZE>& operator=(Buffer<BUFFER_SIZE> const& rhs) {
    memcpy(_buffer, rhs._buffer, BUFFER_SIZE);
    _size = rhs._size;
    return *this;
  }

  constexpr size_t capacity() { return BUFFER_SIZE; }
  uint32_t size() { return _size; }
  uint8_t* buffer() { return _buffer; }
  void zero() { memset(_buffer, 0, BUFFER_SIZE); _size = 0; }
  void set_size(uint32_t size) { _size = size; }

private:
  ALIGNED uint8_t _buffer[BUFFER_SIZE];
  uint32_t _size = 0;
};

