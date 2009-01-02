// controller.cpp
// Author: Allen Porter <allen@thebends.org>

#include "synth/controller.h"

#include <math.h>
#include <assert.h>
#include "synth/envelope.h"
#include "synth/oscillator.h"
#include "synth/modulation.h"

#include <iostream>

namespace synth {

Controller::Controller()
    : volume_envelope_(NULL),
      lfo_(NULL) { }

void Controller::add_oscillator(Oscillator* oscillator) {
  oscillators_.push_back(oscillator);
}

void Controller::set_volume_envelope(Envelope* envelope) {
  volume_envelope_ = envelope;
}

void Controller::set_lfo(LFO* lfo) {
  lfo_ = lfo;
}

void Controller::GetSamples(int num_output_samples, float* output_buffer) {
  for (int i = 0; i < num_output_samples; ++i) {
    output_buffer[i] = GetSample();
  }
}
  
float Controller::GetSample() {
  assert(oscillators_.size() > 0);
  float volume = 1.0;
  if (lfo_) {
    volume *= lfo_->GetValue();
  }
  if (volume_envelope_) {
    volume *= volume_envelope_->GetValue();
  }
  assert(volume >= 0);
  assert(volume <= 1);
  
  // Combine oscillators
  float value = 0;
  for (size_t i = 0; i < oscillators_.size(); ++i) {
    value += oscillators_[i]->GetValue();
  }

  // Clip the combined oscillators
  if (value > 1.0) {
    value = 1.0;
  } else if (value < -1.0) {
    value = -1.0;
  }
  
  float result = value * volume;
  assert(result >= -1);
  assert(result <= 1);
  return result;
}
  

}  // namespace synth
