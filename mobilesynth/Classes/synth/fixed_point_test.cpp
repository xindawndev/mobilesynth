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
  int a = 0x02000000;  // 2.0
  int b = 0x01800000;  // 1.5
  ASSERT_DOUBLE_EQ(3.5, TOFLT(a + b));
  ASSERT_DOUBLE_EQ(0.5, TOFLT(a - b));
} 

void TestMultiply() {
  int a = TOFIX(2.0);
  int b = TOFIX(1.5);
  int c = TOFIX(3.0);
  assert(c == synth::fixed_mult(a, b));
}

void TestMultiply2() {
  float f_v1 = 2.555555f;
  float f_v2 = 1.272727f;
  int v1 = TOFIX(f_v1);
  int v2 = TOFIX(f_v2);
  int q2 = TOFIX(f_v1 * f_v2);
  // Off by one due to rounding error
  assert(q2 == synth::fixed_mult(v1, v2) - 1);
}

void TestMultiply3() {
  ASSERT_DOUBLE_EQ(TOFIX(-0.5), synth::fixed_mult(TOFIX(0.5), TOFIX(-1.0)));
  ASSERT_DOUBLE_EQ(-0.35, TOFLT(synth::fixed_mult(TOFIX(-0.5), TOFIX(0.7))));
}

}  // namespace

int main(int argc, char* argv[]) {
  TestConversion();
  TestAddSubtract();
  TestMultiply();
  TestMultiply2();
  TestMultiply3();
  std::cout << "PASS" << std::endl;
  return 0;
}
