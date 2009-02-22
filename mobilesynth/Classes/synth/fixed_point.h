// fixed_point.h
// Author: Allen Porter <allen@thebends.org>
//
// The iPhone ARM processor does not have an FPU, so we use 8.24 fixed point
// math for generating signals.
//
// In 8.24 fixed point there are eight bits to the left of the radix point and
// 24 bits to the right, forming the fractional portion.  0xWWFFFFFF

namespace synth {

#define TOFIX(d) ((int)((d) * (double)(1 << 24)))
#define TOFLT(a) ((double)(a) / (double)(1 << 24))

static const int kFixedMinus1 = TOFIX(-1.0);
static const int kFixedHalf = TOFIX(0.5);
static const int kFixed1 = TOFIX(1.0);
static const int kFixed2 = TOFIX(2.0);

// 8.24 fixed point multiplication
int32_t fixed_mult(int32_t v1, int32_t v2);
int32_t fixed_floor(int32_t v);

}  // namespace synth
