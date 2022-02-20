/**
 * @brief Generates and stores entropy for TLS connections
 */

#pragma once

#include <array>
#include <stddef.h>
#include <stdint.h>

class EntropyPool {
public:
  EntropyPool(const EntropyPool& other) = delete;
  EntropyPool& operator=(const EntropyPool& rhs) = delete;
  EntropyPool(EntropyPool&& other) = delete;
  EntropyPool& operator=(EntropyPool&& rhs) = delete;

  int get_entropy(uint8_t* outbuf, size_t len, size_t* outlen);

  void add_entropy_to_pool(uint8_t entropy);

  static EntropyPool& instance();

  static void init();

private:
  EntropyPool() {} ;
  ~EntropyPool() {};

  std::array<uint8_t, 1000> _entropy_a;
  std::array<uint8_t, 1000> _entropy_b;

  volatile bool _pool_a_active = true;

  volatile bool _pool_a_filled = false;
  volatile bool _pool_b_filled = false;

  // Pools are filled and consumed from front to back
  // These indicies indicate the next position to be consumed / filled depending on the state of the pool
  volatile uint32_t _pool_a_pos = 0;
  volatile uint32_t _pool_b_pos = 0;

  void trigger_fill();
};
