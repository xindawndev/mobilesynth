// numeric.cpp
// Author: Allen Porter <allen@thebends.org>

#include "numeric.h"

#include <glog/logging.h>

namespace ysynth {

Counter::Counter(long samples, RepeatMode repeat_mode)
  : current_(0),
    samples_(samples - 1),
    repeat_mode_(repeat_mode),
    moving_up_(true) {
  CHECK_LT(0, samples_);
}

Counter::~Counter() { }

float Counter::GetValue() {
  float value = (current_ / (float)samples_);
  if (moving_up_) {
    if (current_ > samples_ - 1 &&
        repeat_mode_ == NONE) {
      return 1.0f;
    }
    current_++;
    if (current_ > samples_ - 1 &&
        repeat_mode_ == MIRRORED_LOOP) {
      moving_up_ = false;
    } else if (current_ > samples_ &&
               repeat_mode_ == LOOP) {
      current_ = 0;
    }
  } else {
    CHECK_EQ(repeat_mode_, MIRRORED_LOOP);
    current_--;
    if (current_ < 1) {
      moving_up_ = true;
    }
  }
  return value;
}

}  // namespace ysynth
