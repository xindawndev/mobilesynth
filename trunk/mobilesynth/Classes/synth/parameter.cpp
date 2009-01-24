// parameter.cpp
// Author: Allen Porter <allen@thebends.org>

#include "synth/parameter.h"
#include <math.h>

namespace synth {

const float Frequency::kOctaveCents(1200);

Frequency::Frequency() : base_frequency_(1.0), offset_cents_(0) {
  reset_frequency();
}

Frequency::~Frequency() { }

float Frequency::GetValue() {
  return frequency_;
}

void Frequency::set_frequency(float frequency) {
  base_frequency_ = frequency;
  reset_frequency();
}

void Frequency::set_offset(int cents) {
  offset_cents_ = cents;
  reset_frequency();
}

void Frequency::reset_frequency() {
  frequency_ = base_frequency_ * powf(2, offset_cents_ / kOctaveCents);
}

// Use A above Middle C as the reference frequency
const int Note::kMiddleAKey(49);
static const float kNotesPerOctave = 12.0;
static const float kMiddleAFrequency = 440.0;

Note::Note() : key_(kMiddleAKey), octave_(NONE) {
  reset_key();
}

Note::~Note() { }

void Note::set_key(int key) {
  key_ = key;
  reset_key();
}

void Note::set_octave(OctaveShift octave) {
  octave_ = octave;
  reset_key();
}

void Note::reset_key() {
  float frequency =
      kMiddleAFrequency *
      powf(2, (key_ - kMiddleAKey) / kNotesPerOctave) *
      octave_;
  set_frequency(frequency);
}

}  // namespace synth
