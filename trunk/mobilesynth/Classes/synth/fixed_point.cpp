// fixed_point.cpp
// Author: Allen Porter <allen@thebends.org>

#include <stdio.h>
#include <stdint.h>

namespace synth {

int32_t fixed_mult(int32_t v1, int32_t v2) {
  return ((int64_t)v1 * v2) >> 24;
}

int32_t fixed_floor(int32_t v) {
  return 0xff000000 & v;
}

}  // namespace synth
