// oscillator.cpp
// Author: Allen Porter <allen@thebends.org>

#include "synth/oscillator.h"
#include <assert.h>
#include <math.h>
#include <iostream>
#include "synth/parameter.h"

namespace synth {

Oscillator::Oscillator()
    : wave_type_(SINE),
      frequency_(NULL),
      sample_rate_(kDefaultSampleRate),
      sample_num_(0) { }

Oscillator::~Oscillator() { }

void Oscillator::set_sample_rate(long sample_rate) {
  sample_rate_ = sample_rate;
}

void Oscillator::set_wave_type(WaveType wave_type) {
  wave_type_ = wave_type;
} 

void Oscillator::set_frequency(Parameter* frequency) {
  frequency_ = frequency;
}

float Oscillator::GetValue() {
  if (frequency_ == NULL) {
    return 0;
  }
  float freq = frequency_->GetValue();
  if (freq == 0) {
    return 0;
  }
  int period_samples = sample_rate_ / freq;
  if (period_samples == 0) {
    return 0;
  }
  float x = (sample_num_ / (float)period_samples);
  float value = 0;
  switch (wave_type_) {
    case SINE:
    case SQUARE:
      value = sinf(2.0 * M_PI * x);
      if (wave_type_ == SQUARE) {
        value = (value > 0) ? 1.0 : -1.0;
      }
      break;
    case TRIANGLE:
      value = (2 * fabs(2.0 * x - 2 * floor(x) - 1) - 1);
      break;
    case SAWTOOTH:
      value = 2 * (x - floor(x) - 0.5);
      break;
    case REVERSE_SAWTOOTH:
      value = 2 * (floor(x) - x + 0.5);
      break;
    default:
      assert(false);
      break;
  }
  sample_num_ = (sample_num_ + 1) % period_samples;
  return value;
}

}  // namespace synth
