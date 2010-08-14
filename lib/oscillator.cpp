// oscillator.cpp
// Author: Allen Porter <allen@thebends.org>

#include "oscillator.h"

#include <glog/logging.h>

namespace ysynth {

Oscillator::Oscillator(long sample_rate,
    Supplier<float>* frequency)
  : sample_num_(0),
    sample_rate_(sample_rate),
    frequency_(frequency) {
  CHECK_LT(0, sample_rate);
  CHECK(frequency != NULL);
}

Oscillator::~Oscillator() { }

float Oscillator::GetValue() {
  long period_samples = sample_rate_ / frequency_->GetValue();
  float value = (sample_num_ / (float)period_samples);
  sample_num_ = (sample_num_ + 1) % period_samples;
  return value;
}

}  // namespace ysynth
