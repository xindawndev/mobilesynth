// filter.h
// Author: Allen Porter <allen@thebends.org>
//
// A filter that passes low-frequency signals.

#ifndef __FILTER_H__
#define __FILTER_H__

namespace synth {

class Filter {
 public:
  Filter();
  virtual ~Filter();

  virtual float GetValue(float t);

 private:
  float rc_;
};

}  // namespace synth

#endif  // __FILTER_H__
