// oscillator.cpp
// Author: Allen Porter <allen@thebends.org>

#include "synth/oscillator.h"
#include <assert.h>
#include <math.h>
#include <iostream>

namespace synth {

Oscillator::Oscillator()
    : wave_type_(SINE),
      level_(1.0),
      frequency_(0),
      cents_(0),
      sample_rate_(kDefaultSampleRate),
      sample_num_(0),
      period_(0) { }

Oscillator::~Oscillator() { }

void Oscillator::set_sample_rate(long sample_rate) {
  sample_rate_ = sample_rate;
  frequency_changed();
}

void Oscillator::set_wave_type(WaveType wave_type) {
  wave_type_ = wave_type;
} 

void Oscillator::set_level(float level) {
  assert(level >= 0.0);
  assert(level <= 1.0);
  level_ = level;
}

void Oscillator::set_frequency(float frequency) {
  assert(frequency >= 0);
  frequency_ = frequency;
  frequency_changed();
}
  
  
void Oscillator::set_frequency_shift(int cents) {
  cents_ = cents;
  frequency_changed();
}

void Oscillator::frequency_changed() {
  // One octave is 1200 cents.  Determine the width of each cent, then
  // calculate the real frequency difference.
  float octave_width = 2 * frequency_ - frequency_;
  float cent_width = octave_width / 12;
  float real_frequency = frequency_ + cents_ * cent_width;
  
  // Determine the number of samples until a full period is reached.  There
  // are sample_rate_ samples per second and the frequency is cycles per second
  // and we want the number of samles per cycle.
  if (real_frequency == 0) {
    period_ = 0;
  } else {
    period_ = (float)sample_rate_ / real_frequency;
  }
  sample_num_ = 0;
}
  
float Oscillator::GetValue() {
  if (period_ == 0) {
    return 0;
  }
  float x = (sample_num_ / (float)period_);
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
  sample_num_ = (sample_num_ + 1) % period_;
  return level_ * value;
}

}  // namespace synth
