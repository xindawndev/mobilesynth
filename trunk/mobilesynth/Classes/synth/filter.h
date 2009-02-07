// filter.h
// Author: Allen Porter <allen@thebends.org>
//
// A filter that passes low-frequency signals.

#ifndef __FILTER_H__
#define __FILTER_H__

#include <vector>
#include "synth/parameter.h"

namespace synth {

// Interface for a filter based on some cutoff frequency (usually high or low
// pass).  The input value is a sample and the output value is a new sample
// at that time.
class Filter {
 public:
  virtual ~Filter();

  // The cutoff frequency of the filter.
  virtual void set_cutoff(Parameter* cutoff) = 0;

  virtual float GetValue(float x) = 0;

 protected:
  Filter();
};

// A simple LowPass FIR filter
class LowPass : public Filter {
 public:
  LowPass();
  virtual ~LowPass();

  virtual void set_cutoff(Parameter* cutoff);

  virtual float GetValue(float x);

 private:
  void reset(float frequency);

  Parameter* cutoff_;
  // Used to keep the last value of the frequency cutoff.  If it changes, we
  // need to re-initialize the filter co-efficients.
  float last_cutoff_;

  // for input value x[k] and output y[k]
  float x1_;  // input value x[k-1]
  float x2_;  // input value x[k-2]
  float y1_;  // output value y[k-1]
  float y2_;  // output value y[k-2]

  // filter coefficients
  float a0_;
  float a1_;
  float a2_;
  float b1_;
  float b2_;
  float b3_;
};

}  // namespace synth

#endif  // __FILTER_H__
