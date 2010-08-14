// osc.h
// Author: Allen Porter <allen@thebends.org>

#include "types.h"

namespace ysynth {

class Wave : public Function<float, float> {
 public:
  virtual ~Wave() { }

  // [0 <= x <= 1]
  virtual float GetValue(float x) = 0;

 protected:
  Wave() { }
};

class Sine : public Wave {
 public:
  virtual float GetValue(float x);
};

class Square : public Wave {
 public:
  virtual float GetValue(float x);
};

class Triangle : public Wave {
 public:
  virtual float GetValue(float x);
};

class Sawtooth : public Wave {
 public:
  virtual float GetValue(float x);
};

class ReverseSawtooth : public Wave {
 public:
  virtual float GetValue(float x);
};

}  // namespace ysynth
