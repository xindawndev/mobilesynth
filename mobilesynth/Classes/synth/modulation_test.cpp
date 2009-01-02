// envelope_test.cpp
// Author: Allen Porter <allen@thebends.org>

#include <assert.h>
#include <math.h>
#include <iostream>
#include <vector>
#include "synth/modulation.h"
#include "synth/oscillator.h"

#define ASSERT_DOUBLE_EQ(x, y) (assert(fabs(x - y) < 0.000001))

namespace {

// An oscillator that actually does not oscillate, so that we can test
// the modulation effect only.
class FakeOscillator : public synth::Oscillator {
 public:
  FakeOscillator() : value_(0.0) { }

  void set_value(float value) {
    value_ = value;
  }

  virtual float GetValue() {
    return value_;
  }

 private:
  float value_;
};

static void TestNoLevel() {
  FakeOscillator osc;
  osc.set_value(0.2);  // ignored

  synth::LFO mod;
  mod.set_level(0.0);
  mod.set_oscillator(&osc);
  ASSERT_DOUBLE_EQ(1.0, mod.GetValue()); 
}

static void TestMaxLevel() {
  FakeOscillator osc;

  synth::LFO mod;
  mod.set_level(1.0);
  mod.set_oscillator(&osc);

  osc.set_value(-1);
  ASSERT_DOUBLE_EQ(0.0, mod.GetValue()); 
  osc.set_value(-0.5);
  ASSERT_DOUBLE_EQ(0.25, mod.GetValue()); 
  osc.set_value(0.0);
  ASSERT_DOUBLE_EQ(0.5, mod.GetValue()); 
  osc.set_value(0.5);
  ASSERT_DOUBLE_EQ(0.75, mod.GetValue()); 
  osc.set_value(1.0);
  ASSERT_DOUBLE_EQ(1.0, mod.GetValue()); 
}

static void TestMidLevel() {
  FakeOscillator osc;

  synth::LFO mod;
  mod.set_level(0.2);
  mod.set_oscillator(&osc);

  osc.set_value(-1);
  ASSERT_DOUBLE_EQ(0.8, mod.GetValue()); 
  osc.set_value(-0.5);
  ASSERT_DOUBLE_EQ(0.85, mod.GetValue()); 
  osc.set_value(0.0);
  ASSERT_DOUBLE_EQ(0.9, mod.GetValue()); 
  osc.set_value(0.5);
  ASSERT_DOUBLE_EQ(0.95, mod.GetValue()); 
  osc.set_value(1.0);
  ASSERT_DOUBLE_EQ(1.0, mod.GetValue()); 
}

}  // namespace

int main(int argc, char* argv[]) {
  TestNoLevel();
  TestMaxLevel();
  TestMidLevel();
  std::cout << "PASS" << std::endl;
  return 0;
}
