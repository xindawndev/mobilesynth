// lag_processor.h
// Author: Allen Porter <allen@thebends.org>
//
// A lag processor, used to implement glide.

#include "synth/parameter.h"
#include "synth/envelope.h"

#ifndef __LAG_PROCESSOR_H__
#define __LAG_PROCESSOR_H__

namespace synth {

class LagProcessor : public Parameter {
 public:
  LagProcessor(Parameter* param);
  virtual ~LagProcessor();

  // The number of samples needed between sample changes
  void set_rate(long samples);
  void set_rate_up(long samples);
  void set_rate_down(long samples);

  virtual float GetValue();

 private:
  long samples_up_;
  long samples_down_;
  Parameter* param_;
  float last_value_;
  long samples_;

  Envelope<float> envelope_;
};

}  // namespace

#endif  // __LOG_PROCESSOR_H__
