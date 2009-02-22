// lag_processor.cpp
// Author: Allen Porter <allen@thebends.org>

#include "synth/lag_processor.h"

#include <iostream>

namespace synth {

LagProcessor::LagProcessor(Parameter* param)
    : samples_up_(0),
      samples_down_(0),
      param_(param),
      last_value_(0) {
  envelope_.set_min(0);
  envelope_.set_max(0);
  envelope_.set_attack(0);
  envelope_.set_decay(0);
  envelope_.set_sustain(0);
  envelope_.set_release(0);
}

LagProcessor::~LagProcessor() { }

void LagProcessor::set_rate(long samples) {
  set_rate_up(samples);
  set_rate_down(samples);
}

void LagProcessor::set_rate_up(long samples) {
  samples_up_ = samples;
}

void LagProcessor::set_rate_down(long samples) {
  samples_down_ = samples;
}

float LagProcessor::GetValue() {
  float value = param_->GetValue();
  if (last_value_ != value) {
    if (last_value_ < value) {
      // Slope up
      envelope_.set_min(last_value_);
      envelope_.set_max(value);
      envelope_.set_attack(samples_up_);
      envelope_.set_decay(0.0); 
      envelope_.set_sustain(value);
      envelope_.set_release(0.0);
      envelope_.NoteOn();
    } else {
      // Slope down
      envelope_.set_max(last_value_);
      envelope_.set_min(value);
      envelope_.set_attack(0);
      envelope_.set_decay(0);
      envelope_.set_sustain(last_value_);
      envelope_.set_release(samples_down_);
      envelope_.NoteOn();
      envelope_.NoteOff();
    }
    last_value_ = value;
  }
  return envelope_.GetValue();
}

}  // namespace synth
