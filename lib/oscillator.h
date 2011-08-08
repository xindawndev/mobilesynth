// osc.h
// Author: Allen Porter <allen@thebends.org>

#include "types.h"

namespace ysynth {

class Gate;

// An oscillator generates a value between 0 and 1 at some particular
// frequency.
class Oscillator : Supplier<float> {
 public:
  Oscillator(long sample_rate,
             Supplier<ControlValue>* frequency,
             Gate* gate);
  virtual ~Oscillator();

  // Output is in the range [0, 1]
  virtual float GetValue();

 private:
  long sample_num_;
  long sample_rate_; 
  Supplier<ControlValue>* frequency_;
  Gate* gate_;
};

}  // namespace ysynth
