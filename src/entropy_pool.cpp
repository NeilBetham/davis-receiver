#include "entropy_pool.h"

#include "registers/adc.h"
#include "registers/sysctl.h"

#include "logging.h"

#include <string.h>

void ADC0Sequence0_ISR() {
  for(uint8_t index = 0; index < 4; index++) {
    uint8_t entropy = ADC_0_SSFIFO0 & 0x0000000F;
    entropy |= (ADC_0_SSFIFO0 & 0x0000000F) << 4;
    EntropyPool::instance().add_entropy_to_pool(entropy);
  }

  ADC_0_ISC |= BIT_0;
}


int EntropyPool::get_entropy(uint8_t* outbuf, size_t len, size_t* outlen) {
  if(_pool_a_active){
    if(_pool_a_filled) {
      uint32_t bytes_left = _entropy_a.size() - _pool_a_pos;
      uint32_t bytes_to_copy = std::min((uint32_t)len, bytes_left);

      memcpy(outbuf, &_entropy_a[_pool_a_pos], bytes_to_copy);
      *outlen = bytes_to_copy;
      _pool_a_pos += bytes_to_copy;

      if(_pool_a_pos == _entropy_a.size()) {
        _pool_a_active = false;
        _pool_a_pos = 0;
        _pool_a_filled = false;
        trigger_fill();
      }

      return 0;
    }
  } else {
    if(_pool_b_filled) {
      uint32_t bytes_left = _entropy_b.size() - _pool_b_pos;
      uint32_t bytes_to_copy = std::min((uint32_t)len, bytes_left);

      memcpy(outbuf, &_entropy_b[_pool_b_pos], bytes_to_copy);
      *outlen = bytes_to_copy;
      _pool_b_pos += bytes_to_copy;

      if(_pool_b_pos == _entropy_b.size()) {
        _pool_a_active = true;
        _pool_b_pos = 0;
        _pool_b_filled = false;
        trigger_fill();
      }

      return 0;
    }
  }

  *outlen = 0;
  return 0;
}

void EntropyPool::add_entropy_to_pool(uint8_t entropy) {
  if(_pool_a_active) {
    if(!_pool_b_filled) {
      _entropy_b[_pool_b_pos] = entropy;
      if((_pool_b_pos + 1) == _entropy_b.size()) {
        _pool_b_filled = true;
        _pool_b_pos = 0;
        log_d("Entropy pool b filled");
      } else {
        _pool_b_pos++;
        trigger_fill();
      }
    }
  } else {
    if(!_pool_a_filled) {
      _entropy_a[_pool_a_pos] = entropy;
      if((_pool_a_pos + 1) == _entropy_a.size()) {
        _pool_a_filled = true;
        _pool_a_pos = 0;
        log_d("Entropy pool a filled");
      } else {
        _pool_a_pos++;
        trigger_fill();
      }
    }
  }
}


EntropyPool& EntropyPool::instance() {
  static EntropyPool instance;
  return instance;
}


void EntropyPool::init() {
  // Setup temp ADC for measurement
  SYSCTL_RCGCADC |= BIT_0;
  ADC_0_ACTSS    =  0;
  ADC_0_EMUX     |= 0x0000EEE0;

  // Configure all the samples in the sequence
  ADC_0_SSEMUX0  =  0;
  ADC_0_SSMUX0   =  0;

  // 8 samples of the interntal temp sensor
  ADC_0_SSCTL0   |= BIT_3 | BIT_7 | BIT_11 | BIT_15 | BIT_19 | BIT_23 | BIT_27 | BIT_29 | BIT_30 | BIT_31;

  // Shortest sample / hold times
  ADC_0_SSTSH0   |= 0x00000000;

  // Enable ints and the sample sequencer 0
  ADC_0_IM       |= BIT_0 | BIT_8;
  ADC_0_ACTSS    |= BIT_0;

  // Trigger the first sequencer iteration
  instance().trigger_fill();

  // Wait for the B pool to fill then set it to active
  while(!instance()._pool_b_filled);
  instance()._pool_a_active = false;
}

void EntropyPool::trigger_fill() {
  ADC_0_PSSI     |= BIT_0;
}
