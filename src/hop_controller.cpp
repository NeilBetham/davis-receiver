#include "hop_controller.h"

#include "entropy_pool.h"

uint32_t HopController::hop() {
  _current_hop = next_index();
  return current_hop();
}

uint32_t HopController::current_hop() {
  return _channels[_hop_pattern[_current_hop]];
}

uint32_t HopController::next_hop() {
  return _channels[_hop_pattern[next_index()]];
}

uint32_t HopController::hop_random() {
  uint8_t random_hop = 0;
  EntropyPool::instance().get_entropy(&random_hop, 1, NULL);
  random_hop %= 52;

  _current_hop = random_hop;
  return current_hop();
}

uint32_t HopController::next_index() {
  uint32_t next_hop_index = _current_hop;
  if(next_hop_index + 1 >= _max_hop) {
    next_hop_index = 0;
  } else {
    next_hop_index++;
  }

  return next_hop_index;
}
