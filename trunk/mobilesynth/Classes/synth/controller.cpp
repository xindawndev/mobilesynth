// controller.cpp
// Author: Allen Porter <allen@thebends.org>

#include "synth/controller.h"

#include <math.h>
#include <assert.h>
#include "synth/envelope.h"
#include "synth/filter.h"
#include "synth/modulation.h"
#include "synth/oscillator.h"

#include <iostream>

namespace synth {

Controller::Controller()
    : volume_envelope_(NULL),
      lfo_(NULL),
      filter_(NULL) { }

void Controller::add_oscillator(Oscillator* oscillator) {
  oscillators_.push_back(oscillator);
}

void Controller::set_volume_envelope(Envelope* envelope) {
  volume_envelope_ = envelope;
}

void Controller::set_filter_envelope(Envelope* envelope) {
  filter_envelope_ = envelope;
}

void Controller::set_lfo(LFO* lfo) {
  lfo_ = lfo;
}

void Controller::set_filter(Filter* filter) {
  filter_ = filter;
}

void Controller::GetFloatSamples(float* buffer, int size) {
  for (int i = 0; i < size; ++i) {
    buffer[i] = GetSample();
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

  // Filter
  if (filter_) {
    assert(value >= -1);
    assert(value <= 1);
    float frequency_offset = filter_envelope_->GetValue();
    filter_->set_offset(frequency_offset);
    value = filter_->GetValue(value);
//    assert(value >= -1);
//    assert(value <= 1);
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
