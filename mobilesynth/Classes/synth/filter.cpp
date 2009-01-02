// filter.cpp
// Author: Allen Porter <allen@thebends.org>

#include "synth/filter.h"

namespace synth {

Filter::Filter()
    : rc_(0) { }

Filter::~Filter() { }

float Filter::GetValue(float t) {
  return 0;
}

}  // namespace synth
