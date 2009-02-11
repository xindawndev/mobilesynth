// filter.cpp
// Author: Allen Porter <allen@thebends.org>

#include "synth/filter.h"
#include <math.h>
#include <iostream>

namespace synth {

static const float kSampleRate = 44100.0;

Filter::Filter() { }

Filter::~Filter() { }

LowPass::LowPass() : cutoff_(NULL), last_cutoff_(0), x1_(0), x2_(0), y1_(0), y2_(0) {
}

LowPass::~LowPass() { }

void LowPass::set_cutoff(Parameter* cutoff) {
  cutoff_ = cutoff;
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

  // TODO(allen): We should probably do more filter passes so that we can ensure  // these stability constraints are met for sufficiently high frequencies.
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
  if (cutoff_ == NULL) {
    return x;
  }

  // Re-initialize the filter co-efficients if they changed
  float cutoff = cutoff_->GetValue();
  if (cutoff < 0) {
    return x;
  } else if (cutoff < 0.001) {
    // Filtering all frequencies
    return 0.0;
  }
  if (fabs(cutoff - last_cutoff_) > 0.001) {
    reset(cutoff);
    last_cutoff_ = cutoff;
  }

  float y = a0_ * x + a1_ * x1_ + a2_ *  x2_ + b1_ * y1_ + b2_ * y2_;
  x1_ = x;
  x2_ = x1_;
  y2_ = y1_;
  y1_ = y;
  return y;
}

FilterCutoff::FilterCutoff() : cutoff_(-1),
                               modulation_(NULL) { }

FilterCutoff::~FilterCutoff() { }

float FilterCutoff::GetValue() {
  float value = cutoff_ * envelope_.GetValue();
  if (modulation_) {
    value *= modulation_->GetValue();
  }
  return value;
}

}  // namespace synth
