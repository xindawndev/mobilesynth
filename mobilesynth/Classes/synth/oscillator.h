// oscillator.h
// Author: Allen Porter <allen@thebends.org>
//
// An oscillator generates a signal with a particular frequency and amplitude.

#ifndef __OSCILLATOR_H__
#define __OSCILLATOR_H__

namespace synth {
  
static const long kDefaultSampleRate = 44100;

class Oscillator {
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

  // Override the default sample rate
  void set_sample_rate(long sample_rate);

  // Amplitude, ranges between 0 and 1
  void set_level(float level);

  // Frequency at which to oscillate
  void set_frequency(float frequency);
  
  // A fine tune adjustment to the frequency.  Can be fixed as the base
  // frequency is shifted.
  void set_frequency_shift(int cents);

  // Returns the value at the specific time [0.0, 1.0].  The returned value
  // returned value is in the range [-1.0, 1.0].
  virtual float GetValue();

 protected:
  float level() { return level_; }
  void frequency_changed();

 private:
  WaveType wave_type_;
  float level_;
  float frequency_;
  float cents_;

  long sample_rate_;  
  long sample_num_;
  long period_;
};

}  // namespace synth

#endif  // __OSCILLATOR_H__
