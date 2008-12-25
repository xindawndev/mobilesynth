// modulation.h
// Author: Allen Porter <allen@thebends.org>

#ifndef __MODULATION_H__
#define __MODULATION_H__

namespace synth {

class Oscillator;

class LFO {
 public:
  LFO();

  // Set the amount of modulation from [0, 1].
  void set_level(float level);

  // The specified oscillator should have its level set to 1
  void set_oscillator(Oscillator* oscillator);

  // Returns an amplitude multiplier from [0, 1].
  float GetValue(float t);

 private:
  float level_;
  Oscillator* oscillator_;
};

}  // namespace synth

#endif  // __MODULATION_H__
