// modualtion.cpp
// Author: Allen Porter <allen@thebends.org>

#include "synth/modulation.h"
#include <assert.h>
#include <sys/param.h>
#include "synth/oscillator.h"
#include <iostream>

namespace synth {

LFO::LFO()
    : level_(0),
      oscillator_(NULL)
      { }

void LFO::set_level(float level) {
  level_ = level;
}

void LFO::set_oscillator(Oscillator* oscillator) {
  oscillator_ = oscillator;
}

// The oscillator affects the amplitude more as the level is increased, or
// not at all if the level is zero.
float LFO::GetValue() {
  float m = 0.5 * level_;
  float b = 1.0 - m;
  float value =  m * oscillator_->GetValue() + b;
  assert(value >= 0);
  assert(value <= 1.0);
  return value;
}

}  // namespace synth
