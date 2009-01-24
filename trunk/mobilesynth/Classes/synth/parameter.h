// parameter.h
// Author: Allen Porter <allen@thebends.org>
//
// The parameter interface is used to abstract the notion of a setting of a
// another module (such as an Oscillator or Filter) that may change over time.
//
// For example, the frequency parameter of an oscillator can change via:
// - A note being struck
// - Glide setting that controls the time to move from one note to another
// - An "octave" setting
// - A fine tune parameter
// The parameter lets us separate the way we determine the frequency from the
// module that is interested in using the value.

#ifndef __PARAMETER_H__
#define __PARAMETER_H__

namespace synth {
  
class Parameter {
 public:
  virtual ~Parameter() { }

  // Return the current value of the parameter.  Parameters that change over
  // time expect to be invoked once for every sample.  All modules assume they
  // are using the same default sample rate.
  virtual float GetValue() = 0;

 protected:
  Parameter() { }
};

class Frequency : public Parameter {
 public:
  static const float kOctaveCents;

  Frequency();
  virtual ~Frequency();

  virtual float GetValue();  

  // Set the base frequency in Hz.
  void set_frequency(float frequency);

  // Fine tuning adjustment added to the frequency.  The offset is the number of
  // cents to shift from the frequency.  There are kOctaveCents per octave.
  void set_offset(int cents);

 private:
  // Update the frequency.  Should be called when any parameters change.
  void reset_frequency();

  float base_frequency_;
  float offset_cents_;

  // Calculated from base_frequency + offset
  float frequency_;
};

class Note : public Frequency {
 public:
  // The key number of Middle A (the A above Middle C).
  static const int kMiddleAKey;

  Note();
  virtual ~Note();

  // The key number of the note.  Keys are integers typically between 1 and 88.
  void set_key(int key);

  // A shift in frequency by the specified amount.  The frequency gets
  // multiplied by 2^n
  enum OctaveShift {
    NONE = 1,
    OCTAVE_2 = 2,
    OCTAVE_4 = 4,
    OCTAVE_8 = 8,
    OCTAVE_16 = 16
  };

  // Play the note at the specified octave shift instead of the key
  // specified.
  void set_octave(OctaveShift octave);

 private:
  // Update the frequency.  Should be called when any parameters change.
  void reset_key();

  int key_;
  OctaveShift octave_;
};

}  // namespce synth

#endif  // __PARAMETER_H__
