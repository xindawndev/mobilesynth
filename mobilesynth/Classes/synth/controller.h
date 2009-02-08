// controller.h
// Author: Allen Porter <allen@thebends.org>
//
// The controller module returns samples and drives the oscillator and envelope.

#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <vector>
#include "synth/envelope.h"
#include "synth/filter.h"
#include "synth/lag_processor.h"
#include "synth/modulation.h"
#include "synth/oscillator.h"
#include "synth/parameter.h"

namespace synth {

class Controller {
 public:
  Controller();

  // Volume [0, 1.0]
  void set_volume(float volume);

  // Start/Stop playing a note.  These trigger the Attack and Release of the
  // volume and filter envelopes.
  void NoteOn(int midi_note);
  void NoteOnFrequency(float frequency);  // For testing
  void NoteChange(int midi_note);  // Note changes without release
  void NoteOff();

  void set_sample_rate(float sample_rate);  // For testing

  // A shift in frequency by the specified amount.  The frequency gets
  // multiplied by 2^n
  enum OctaveShift {
    OCTAVE_1 = 1,  // No shift
    OCTAVE_2 = 2,
    OCTAVE_4 = 4,
    OCTAVE_8 = 8,
    OCTAVE_16 = 16
  };

  // OSC 1

  // Set the volume of oscillator
  void set_osc1_level(float level);
  // Set the wave form of oscillator
  void set_osc1_wave_type(Oscillator::WaveType wave_type);
  // The oscillator frequency is shifted by the specified amount.
  void set_osc1_octave(OctaveShift octave);

  // OSC 2
  void set_osc2_level(float level);
  void set_osc2_wave_type(Oscillator::WaveType wave_type);
  void set_osc2_octave(OctaveShift octave);
  void set_osc2_shift(int cents);

  void set_osc_sync(bool sync);

  Envelope* volume_envelope() { return &volume_envelope_; }
  Envelope* filter_envelope() { return &filter_envelope_; }

  enum ModulationSource {
    LFO_SRC_SQUARE,
    LFO_SRC_TRIANGLE,
    LFO_SRC_SAWTOOTH,
    LFO_SRC_REVERSE_SAWTOOTH,
// TODO(allen): How do you determine the sustain length?
// Is it sustain = period - (attack + decay + release)?
//    LFO_FILTER_ENVELOPE,
// TODO(allen): OSC2 needs a manual frequency control for this to work, i think.
//    OSC2,
  };
  enum ModulationDestination {
    LFO_DEST_WAVE,  // Tremelo
    LFO_DEST_PITCH,  // Vibrato
    LFO_DEST_FILTER,
    // TODO(allen): Is this Ring modulation?
    // LFO_DEST_OSC2,
  };
  void set_modulation_source(ModulationSource source);
  void set_modulation_destination(ModulationDestination dest);
  void set_modulation_amount(float amount);
  void set_modulation_frequency(float frequency);

  void set_filter_cutoff(float frequency);

  void set_glide_rate(long samples);

  // Get a single sample
  float GetSample();

  void GetFloatSamples(float* buffer, int size);

 private:
  // Invoked when one of the routing parameters changes, such as the source
  // or destination of modulation.
  void reset_routing();

  MutableParameter keyboard_frequency_;
  // Keyboard frequency plus glide
  LagProcessor key_frequency_;
  // Keyboard frequency, including any modulation
  MultiplyParameter frequency_;

  // Combine the key_frequency_ and the oscillator shifts into the frequencies
  // for each oscillator.
  MutableParameter osc1_octave_shift_;
  MultiplyParameter osc1_frequency_;
  
  MutableParameter osc2_fine_;
  MutableParameter osc2_octave_shift_;
  MultiplyParameter osc2_frequency_;

  Oscillator osc1_;
  MutableParameter osc1_level_;
  MultiplyParameter osc1_output_;

  Oscillator osc2_;
  MutableParameter osc2_level_;
  MultiplyParameter osc2_output_;

  SumParameter osc_;
  MutableParameter volume_;
  Envelope volume_envelope_;
  MultiplyParameter wave_;

  bool osc_sync_;

  ModulationSource modulation_source_;
  ModulationDestination modulation_destination_;
  MutableParameter modulation_frequency_;
  Oscillator modulation_osc_;
  MutableParameter modulation_amount_;
  LFO modulation_;

  MutableParameter filter_cutoff_;
  MultiplyParameter filter_cutoff_total_;
  Envelope filter_envelope_;
  LowPass filter_;
};

}  // namespace synth

#endif // __CONTROLLER_H__
