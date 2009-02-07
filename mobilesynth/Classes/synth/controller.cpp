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
  
const float kOctaveCents(1200);

static const int kMiddleAKey(49);
static const float kNotesPerOctave = 12.0;
static const float kMiddleAFrequency = 440.0;

static float KeyToFrequency(int key) {
  return kMiddleAFrequency * powf(2, (key - kMiddleAKey) / kNotesPerOctave);
}

Controller::Controller()
    : keyboard_frequency_(0),
      key_frequency_(&keyboard_frequency_),
      osc1_octave_shift_(OCTAVE_1),
      osc2_fine_(1.0),
      osc2_octave_shift_(OCTAVE_1),
      osc1_level_(0.0),
      osc2_level_(0.0),
      volume_(1.0),
      osc_sync_(false),
      modulation_source_(LFO_SRC_SQUARE),
      modulation_destination_(LFO_DEST_WAVE),
      modulation_frequency_(0),
      modulation_amount_(0),
      filter_cutoff_(0.0) {
  // Osc1: Combine the keyboard frequency (post glide) and the oscillator shift
  osc1_frequency_.AddParameter(&key_frequency_);
  osc1_frequency_.AddParameter(&osc1_octave_shift_);
  osc1_.set_frequency(&osc1_frequency_);
  // Osc2: Combine the keyboard frequency (post glide) and the oscillator shift
  osc2_frequency_.AddParameter(&key_frequency_);
  osc2_frequency_.AddParameter(&osc2_fine_);  // fine tuning
  osc2_frequency_.AddParameter(&osc2_octave_shift_);
  osc2_.set_frequency(&osc2_frequency_);
  // Oscillator levels
  osc1_output_.AddParameter(&osc1_);
  osc1_output_.AddParameter(&osc1_level_);
  osc2_output_.AddParameter(&osc2_);
  osc2_output_.AddParameter(&osc2_level_);
  // Sum the oscillators
  osc_.AddParameter(&osc1_output_);
  osc_.AddParameter(&osc2_output_);
        
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
  keyboard_frequency_.set_value(frequency);
  volume_envelope_.NoteOn();
  filter_envelope_.NoteOn();
}

void Controller::NoteChange(int note) {
  assert(note >= 1);
  assert(note <= 88);
  keyboard_frequency_.set_value(KeyToFrequency(note));
}

void Controller::NoteOff() {
  volume_envelope_.NoteOff();
  filter_envelope_.NoteOff();
}

void Controller::set_osc1_level(float level) {
  osc1_level_.set_value(level);
}

void Controller::set_osc1_wave_type(Oscillator::WaveType wave_type) {
  osc1_.set_wave_type(wave_type);
}

void Controller::set_osc1_octave(OctaveShift octave) {
  osc1_octave_shift_.set_value(octave);
}

void Controller::set_osc2_level(float level) {
  osc2_level_.set_value(level);
}

void Controller::set_osc2_wave_type(Oscillator::WaveType wave_type) {
  osc2_.set_wave_type(wave_type);
}

void Controller::set_osc2_octave(OctaveShift octave) {
  osc2_octave_shift_.set_value(octave);
}
  
void Controller::set_osc2_shift(int cents) {
  osc2_fine_.set_value(powf(2, cents / kOctaveCents));
}

void Controller::set_osc_sync(bool sync) {
  osc_sync_ = sync;
}
  
void Controller::set_glide_rate(long rate) {
  key_frequency_.set_rate(rate);
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
  wave_.AddParameter(&osc_);
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

  filter_cutoff_total_.Clear();
  filter_cutoff_total_.AddParameter(&filter_cutoff_);
  filter_cutoff_total_.AddParameter(&filter_envelope_);

  // Route modulation into the correct pipeline
  switch (modulation_destination_) {
    case LFO_DEST_WAVE:
      // Modulate the volume
      wave_.AddParameter(&modulation_);
      break;
    case LFO_DEST_PITCH:
      // Modulate the frequency
      // TODO(allen): adjust pitch -- what does that mean?
      //osc2_frequency_.AddParameter(&modulation_);
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
  if (osc_sync_) {
    // If the first oscillator is at the start of its period, reset the second
    // oscillator so that it is also at the start of its period.
    if (osc1_.IsStart()) {
      osc2_.Reset();
    }
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
