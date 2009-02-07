// oscillator.h
// Author: Allen Porter <allen@thebends.org>
//
// An oscillator generates a signal with a particular frequency and amplitude.

#ifndef __OSCILLATOR_H__
#define __OSCILLATOR_H__

#include "synth/parameter.h"

namespace synth {

static const long kDefaultSampleRate = 44100;

class Oscillator : public Parameter {
 public:
  Oscillator();
  virtual ~Oscillator();

  enum WaveType {
    SINE,
    SQUARE,
    TRIANGLE,
    SAWTOOTH,
    REVERSE_SAWTOOTH,
  };
  void set_wave_type(WaveType wave_type);

  void set_frequency(Parameter* frequency);

  // Override the default sample rate
  void set_sample_rate(long sample_rate);

  // Returns the value at the specific time [0.0, 1.0].  The returned value
  // returned value is in the range [-1.0, 1.0].
  virtual float GetValue();

  // Start at the beginning of the period
  void Reset() { sample_num_ = 0; }

  // Returns true if this is the first sample in the period
  bool IsStart() { return (sample_num_ == 0); }

 private:
  WaveType wave_type_;
  Parameter* level_;
  Parameter* frequency_;

  long sample_rate_;  
  long sample_num_;
};

}  // namespace synth

#endif  // __OSCILLATOR_H__
