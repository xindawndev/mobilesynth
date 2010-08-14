// numeric.h
// Author: Allen Porter <allen@thebends.org>

#include "types.h"
#include <glog/logging.h>

namespace ysynth {

enum RepeatMode {
  NONE,
  LOOP,
  MIRRORED_LOOP,
};

class Counter : public Supplier<float> {
 public:
  Counter(long samples, RepeatMode repeat_mode);
  virtual ~Counter();

  virtual float GetValue();

 private:
  long current_;
  long samples_;
  RepeatMode repeat_mode_;
  bool moving_up_;
};

template<typename T>
class Interpolation : public Supplier<T> {
 public:
  Interpolation(T start,
                T end,
                long samples,
                RepeatMode repeat_mode)
    : counter_(samples, repeat_mode),
      start_(start),
      width_(end - start) {
  }

  virtual ~Interpolation() { }

  virtual T GetValue() {
    return start_ + counter_.GetValue() * width_;
  }

 private:
  Counter counter_;
  T start_;
  T width_;
};

}  // namespace ysynth
