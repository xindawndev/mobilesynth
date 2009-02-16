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

// 8.24 fixed point multiplication
int fixed_mult(int v1, int v2);

}  // namespace synth
