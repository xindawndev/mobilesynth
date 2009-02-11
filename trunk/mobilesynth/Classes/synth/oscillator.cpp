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
    return 0.0f;
  }
  float freq = frequency_->GetValue();
  if (freq < 0.01f) {
    return 0.0f;
  }
  long period_samples = sample_rate_ / freq;
  float x = (sample_num_ / (float)period_samples);
  float value = 0;
  switch (wave_type_) {
    case SINE:
      value = sinf(2.0f * M_PI * x);
      break;
    case SQUARE:
      if (sample_num_ < (period_samples / 2)) {
        value = 1.0f;
      } else {
        value = -1.0f;
      }
      break;
    case TRIANGLE:
      value = (2.0f * fabs(2.0f * x - 2.0f * floor(x) - 1.0f) - 1.0f);
      break;
    case SAWTOOTH:
      value = 2.0f * (x - floor(x) - 0.5f);
      break;
    case REVERSE_SAWTOOTH:
      value = 2.0f * (floor(x) - x + 0.5f);
      break;
    default:
      assert(false);
      break;
  }
  sample_num_ = (sample_num_ + 1) % period_samples;
  return value;
}


KeyboardOscillator::KeyboardOscillator(Oscillator* osc1, Oscillator* osc2)
  : base_frequency_(0),
    key_frequency_(&base_frequency_),
    osc1_octave_(1),
    osc2_octave_(1),
    osc1_level_(0),
    osc2_level_(0),
    osc2_shift_(0),
    osc1_freq_(0),
    osc2_freq_(0),
    frequency_modulation_(NULL),
    sync_(false),
    osc1_(osc1),
    osc2_(osc2) {
  osc1_->set_frequency(&osc1_freq_);
  osc2_->set_frequency(&osc2_freq_);
}

const float kOctaveCents(1200);

KeyboardOscillator::~KeyboardOscillator() { }

float KeyboardOscillator::GetValue() {
  float root_note = key_frequency_.GetValue(); 
  if (frequency_modulation_) {
    root_note *= frequency_modulation_->GetValue();
  }
  osc1_freq_.set_value(root_note * osc1_octave_);
  float osc2_freq = root_note * osc2_octave_;
  if (osc2_shift_ != 0) {
    if (osc2_shift_ != last_osc2_shift_) {
      osc2_shift_multiple_ = powf(2, osc2_shift_ / kOctaveCents);
      last_osc2_shift_ = osc2_shift_;
    }
    osc2_freq *= osc2_shift_multiple_;
  }
  osc2_freq_.set_value(osc2_freq);

  float value = osc1_level_ * osc1_->GetValue() +
                osc2_level_ * osc2_->GetValue();
  // Clip
  value = fminf(value, 1.0f);
  return fmaxf(value, -1.0f);
}

}  // namespace synth
