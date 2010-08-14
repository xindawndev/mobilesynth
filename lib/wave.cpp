// osc.cpp
// Author: Allen Porter <allen@thebends.org>

#include "wave.h"

#include <glog/logging.h>
#include <math.h>

namespace ysynth {

float Sine::GetValue(float x) {
  CHECK_LE(0, x);
  CHECK_GE(1.0, x);
  return sinf(2.0f * M_PI * x);
}

float Square::GetValue(float x) {
  CHECK_LE(0, x);
  CHECK_GE(1.0, x);
  return (x < 0.5f) ? 1.0f : -1.0f;
}

float Triangle::GetValue(float x) {
  CHECK_LE(0, x);
  CHECK_GE(1.0, x);
  return (2.0f * fabs(2.0f * x - 2.0f * floorf(x) - 1.0f) - 1.0f);
}

float Sawtooth::GetValue(float x) {
  CHECK_LE(0, x);
  CHECK_GE(1.0, x);
  return 2.0f * (x - floorf(x) - 0.5f);
}

float ReverseSawtooth::GetValue(float x) {
  CHECK_LE(0, x);
  CHECK_GE(1.0, x);
  return 2.0f * (floorf(x) - x + 0.5f);
}

}  // namespace ysynth
