// fixed_point_test.cpp
// Author: Allen Porter <allen@thebends.org>

#include <assert.h>
#include <iostream>
#include "synth/fixed_point.h"
#include "synth/test_util.h"

namespace {

void TestConversion() {
  assert(0x02000000L == TOFIX(2.0));
  assert(0x01000000L == TOFIX(1.0));
  assert(0x00800000L == TOFIX(0.5));
  assert(0x00400000L == TOFIX(0.25));

  ASSERT_DOUBLE_EQ(2.0, TOFLT(0x02000000));
  ASSERT_DOUBLE_EQ(1.0, TOFLT(0x01000000));
  ASSERT_DOUBLE_EQ(0.5, TOFLT(0x00800000));
}

void TestAddSubtract() {
  int32_t a = 0x02000000;  // 2.0
  int32_t b = 0x01800000;  // 1.5
  ASSERT_DOUBLE_EQ(3.5, TOFLT(a + b));
  ASSERT_DOUBLE_EQ(0.5, TOFLT(a - b));
} 

void TestMultiply() {
  int32_t a = TOFIX(2.0);
  int32_t b = TOFIX(1.5);
  int32_t c = TOFIX(3.0);
  assert(c == synth::fixed_mult(a, b));
}

void TestMultiply2() {
  float f_v1 = 2.555555f;
  float f_v2 = 1.272727f;
  int32_t v1 = TOFIX(f_v1);
  int32_t v2 = TOFIX(f_v2);
  int32_t q2 = TOFIX(f_v1 * f_v2);
  // Off by one due to rounding error
  assert(q2 == synth::fixed_mult(v1, v2) - 1);
}

void TestMultiply3() {
std::cout << "=> " << synth::fixed_mult(TOFIX(0.5), TOFIX(-1.0)) << std::endl;
std::cout << "=> " << TOFLT(synth::fixed_mult(TOFIX(0.5), TOFIX(-1.0))) << std::endl;

  ASSERT_DOUBLE_EQ(TOFIX(-0.5), synth::fixed_mult(TOFIX(0.5), TOFIX(-1.0)));
  ASSERT_DOUBLE_EQ(-0.35, TOFLT(synth::fixed_mult(TOFIX(-0.5), TOFIX(0.7))));
}

void TestMultiply4() {
  int32_t a = TOFIX(1.0);
  int32_t b = TOFIX(0.9);
  int32_t c = TOFIX(0.9);
  assert(c == synth::fixed_mult(a, b));
}

void TestFloor() {
  float values[] = { 2.476, 1.99, 1.0, 0.123, 0.0, -0.01, -1.32, -2.2 };
  int len = sizeof(values) / sizeof(float);
  for (int i = 0; i < len; ++i) {
    ASSERT_DOUBLE_EQ(floorf(values[i]),
                     TOFLT(synth::fixed_floor(TOFIX(values[i]))));
  }
}

}  // namespace

int main(int argc, char* argv[]) {
  TestConversion();
  TestAddSubtract();
  TestMultiply();
  TestMultiply2();
  TestMultiply3();
  TestMultiply4();
  TestFloor();
  std::cout << "PASS" << std::endl;
  return 0;
}
