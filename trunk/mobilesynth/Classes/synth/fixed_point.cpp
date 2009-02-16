// fixed_point.cpp
// Author: Allen Porter <allen@thebends.org>

namespace synth {

static void fixed_multiply(int v1, int v2, int* result_hi, int* result_lo) {
  // Multiple v1 and v2 into a 64 bit product (hi, lo)
  int a = v1 >> 16;
  int b = v1 & 0xffff;
  int c = v2 >> 16;
  int d = v2 & 0xffff;

  int lo = b * d;
  int x = a * d + c * b;  // AD + CB
  int y = (lo >> 16) + x;
  lo = (lo & 0xffff) | (y << 16);
  int hi = (y >> 16);
  hi += a * c;

  *result_hi = hi;
  *result_lo = lo;
}

int fixed_mult(int v1, int v2) {
  int hi;
  int lo;
  fixed_multiply(v1, v2, &hi, &lo);
  return (hi << 8) | (lo >> 24);
}

}  // namespace synth
