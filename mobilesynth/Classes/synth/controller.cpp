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
    : sample_rate_(0),
      sample_(0),
      volume_(1.0),
      oscillator_(NULL),
      volume_envelope_(NULL),
      lfo_(NULL) { }

int Controller::sample_rate() {
  return sample_rate_;
}

void Controller::set_sample_rate(int sample_rate) {
  sample_rate_ = sample_rate;
}

void Controller::set_volume(float volume) {
  assert(volume >= 0.0);
  assert(volume <= 1.0);
  volume_ = volume;
}

float Controller::volume() {
  return volume_;
}

void Controller::set_oscillator(Oscillator* oscillator) {
  oscillator_ = oscillator;
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
  assert(sample_rate_ > 0);
  assert(oscillator_);
  assert(volume_envelope_);
  
  sample_ = (sample_ + 1) % sample_rate_;
  float t = sample_ / (float)sample_rate_;
  float value = volume_ * volume_envelope_->GetValue() *
         oscillator_->GetValue(t) * lfo_->GetValue(t);
  assert(value >= -1);
  assert(value <= 1);
  return value;
}
  

}  // namespace synth
