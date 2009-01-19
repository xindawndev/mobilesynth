// filter.cpp
// Author: Allen Porter <allen@thebends.org>

#include "synth/filter.h"
#include <math.h>
#include <iostream>

namespace synth {

static const float kSampleRate = 44100.0;

Filter::Filter() { }

Filter::~Filter() { }

LowPass::LowPass() : cutoff_(0), offset_(0){
}

LowPass::~LowPass() { }

void LowPass::set_cutoff(float frequency) {
  cutoff_ = frequency;  
  reset(cutoff_ + offset_);
}

void LowPass::set_offset(float amount) {
  assert(amount <= 1);
  assert(amount >= 0);
  // TODO(allen): How do you come up with the max value?
  offset_ = amount * 10000;
  reset(cutoff_ + offset_);
}

void LowPass::reset(float frequency) {
  // Number of filter passes
  float n = 1;

  // 3dB cutoff frequency
  float f0 = frequency;

  // Sample frequency
  const float fs = 44100.0;

  // 3dB cutoff correction
  float c = powf(powf(2, 1.0f / n) - 1, -0.25);

  // Polynomial coefficients
  float g = 1;
  float p = sqrtf(2);

  // Corrected cutoff frequency
  float fp = c * (f0 / fs);

  // Ensure fs is OK for stability constraint
  //assert(fp > 0);
  //assert(fp < 0.125);

  // Warp cutoff freq from analog to digital domain
  float w0 = tanf(M_PI * fp);

  // Calculate the filter co-efficients
  float k1 = p * w0;
  float k2 = g * w0 * w0;

  a0_ = k2 / (1 + k1 + k2);
  a1_ = 2 * a0_;
  a2_ = a0_;
  b1_ = 2 * a0_ * (1 / k2 - 1);
  b2_ = 1 - (a0_ + a1_ + a2_ + b1_);
}

float LowPass::GetValue(float x) {
  float y = a0_ * x + a1_ * x1_ + a2_ *  x2_ + b1_ * y1_ + b2_ * y2_;
  x1_ = x;
  x2_ = x1_;
  y2_ = y1_;
  y1_ = y;
  return y;
}

}  // namespace synth
