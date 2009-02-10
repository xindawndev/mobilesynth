// controller.cpp
// Author: Allen Porter <allen@thebends.org>

#include "synth/controller.h"

#include <math.h>
#include <assert.h>
#include <iostream>
#include "synth/envelope.h"
#include "synth/filter.h"
#include "synth/modulation.h"
#include "synth/oscillator.h"

namespace synth {
  
static const int kMiddleAKey(49);
static const float kNotesPerOctave = 12.0;
static const float kMiddleAFrequency = 440.0;

static float KeyToFrequency(int key) {
  return kMiddleAFrequency * powf(2, (key - kMiddleAKey) / kNotesPerOctave);
}

Controller::Controller()
    : combined_osc_(&osc1_, &osc2_),
      volume_(1.0),
      osc_sync_(false),
      modulation_source_(LFO_SRC_SQUARE),
      modulation_destination_(LFO_DEST_WAVE),
      modulation_frequency_(0),
      modulation_amount_(0),
      filter_cutoff_(-1.0) {
  modulation_osc_.set_frequency(&modulation_frequency_);
  modulation_.set_oscillator(&modulation_osc_);
  modulation_.set_level(&modulation_amount_);

  filter_.set_cutoff(&filter_cutoff_total_);

  reset_routing();
}

void Controller::set_volume(float volume) {
  volume_.set_value(volume);
}

void Controller::set_sample_rate(float sample_rate) {
  osc1_.set_sample_rate(sample_rate);
  osc2_.set_sample_rate(sample_rate);
}

void Controller::NoteOn(int note) {
  assert(note >= 1);
  assert(note <= 88);
  NoteOnFrequency(KeyToFrequency(note));
}

void Controller::NoteOnFrequency(float frequency) {
  combined_osc_.set_keyboard_frequency(frequency);
  volume_envelope_.NoteOn();
  filter_envelope_.NoteOn();
}

void Controller::NoteChange(int note) {
  assert(note >= 1);
  assert(note <= 88);
  combined_osc_.set_keyboard_frequency(KeyToFrequency(note));
}

void Controller::NoteOff() {
  volume_envelope_.NoteOff();
  filter_envelope_.NoteOff();
}

void Controller::set_osc1_level(float level) {
  combined_osc_.set_osc1_level(level);
}

void Controller::set_osc1_wave_type(Oscillator::WaveType wave_type) {
  osc1_.set_wave_type(wave_type);
}

void Controller::set_osc1_octave(OctaveShift octave) {
  combined_osc_.set_osc1_octave((int)octave);
}

void Controller::set_osc2_level(float level) {
  combined_osc_.set_osc2_level(level);
}

void Controller::set_osc2_wave_type(Oscillator::WaveType wave_type) {
  osc2_.set_wave_type(wave_type);
}

void Controller::set_osc2_octave(OctaveShift octave) {
  combined_osc_.set_osc2_octave((int)octave);
}
  
void Controller::set_osc2_shift(int cents) {
  combined_osc_.set_osc2_shift(cents);
}

void Controller::set_osc_sync(bool sync) {
  combined_osc_.set_osc_sync(sync);
}
  
void Controller::set_glide_rate(long rate) {
  combined_osc_.set_glide_rate(rate);
}

void Controller::set_modulation_amount(float amount) {
  modulation_amount_.set_value(amount);
}

void Controller::set_modulation_frequency(float frequency) {
  modulation_frequency_.set_value(frequency);
}

void Controller::set_modulation_source(ModulationSource src) {
  modulation_source_ = src;
  reset_routing();
}

void Controller::set_modulation_destination(ModulationDestination dest) {
  modulation_destination_ = dest;
  reset_routing();
}

void Controller::reset_routing() {
  wave_.Clear();
  wave_.AddParameter(&combined_osc_);
  wave_.AddParameter(&volume_envelope_);
  wave_.AddParameter(&volume_);

  switch (modulation_source_) {
    case LFO_SRC_SQUARE:
      modulation_osc_.set_wave_type(Oscillator::SQUARE);
      break;
    case LFO_SRC_TRIANGLE:
      modulation_osc_.set_wave_type(Oscillator::TRIANGLE);
      break;
    case LFO_SRC_SAWTOOTH:
      modulation_osc_.set_wave_type(Oscillator::SAWTOOTH);
      break;
    case LFO_SRC_REVERSE_SAWTOOTH:
      modulation_osc_.set_wave_type(Oscillator::REVERSE_SAWTOOTH);
      break;
    default:
      assert(false);
  }

  // Reset the destinations
  filter_cutoff_total_.Clear();
  filter_cutoff_total_.AddParameter(&filter_cutoff_);
  filter_cutoff_total_.AddParameter(&filter_envelope_);

  combined_osc_.set_frequency_modulation(NULL);

  // Route modulation into the correct pipeline
  switch (modulation_destination_) {
    case LFO_DEST_WAVE:
      // Modulate the volume (tremelo)
      wave_.AddParameter(&modulation_);
      break;
    case LFO_DEST_PITCH:
      // Modulate the frequency (vibrato)
      combined_osc_.set_frequency_modulation(&modulation_);
      break;
    case LFO_DEST_FILTER:
      // Modulate the cutoff frequency
      filter_cutoff_total_.AddParameter(&modulation_);
      break;
    default:
      assert(false);
  }
}

void Controller::set_filter_cutoff(float frequency) {
  filter_cutoff_.set_value(frequency);
}

void Controller::GetFloatSamples(float* buffer, int size) {
  for (int i = 0; i < size; ++i) {
    buffer[i] = GetSample();
  }
}

float Controller::GetSample() {
  if (volume_envelope_.released() || filter_envelope_.released()) {
    return 0;
  }

  // Combined oscillators, volume/envelope/modulation
  float value = wave_.GetValue();

  // Clip!
  value = fmax(-1, value);
  value = fmin(1, value);

  // Combined filter with envelope/modulation
  value = filter_.GetValue(value);

  // Clip!
  value = fmax(-1, value);
  value = fmin(1, value);

  return value;
}
  

}  // namespace synth
