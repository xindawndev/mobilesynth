// controller_test.cpp
// Author: Allen Porter <allen@thebends.org>

#include <assert.h>
#include <iostream>
#include <vector>
#include "synth/controller.h"
#include "synth/envelope.h"
#include "synth/modulation.h"
#include "synth/oscillator.h"
#include "synth/test_util.h"

namespace {

static void TestController() {
  synth::Oscillator osc;
  osc.set_sample_rate(16);
  osc.set_wave_type(synth::Oscillator::TRIANGLE);
  osc.set_level(0.5);
  osc.set_frequency(2);  // two cycles per second

  synth::Envelope env;
  env.set_attack(0);
  env.set_decay(0);
  env.set_sustain(1.0);
  env.set_release(0);

  synth::Controller controller;
  controller.add_oscillator(&osc);
  controller.set_volume_envelope(&env);
  env.NoteOn();

  for (int i = 0; i < 10; ++i) {
    ASSERT_DOUBLE_EQ(0.5, controller.GetSample());
    ASSERT_DOUBLE_EQ(0.25, controller.GetSample());
    ASSERT_DOUBLE_EQ(0, controller.GetSample());
    ASSERT_DOUBLE_EQ(-0.25, controller.GetSample());
    ASSERT_DOUBLE_EQ(-0.5, controller.GetSample());
    ASSERT_DOUBLE_EQ(-0.25, controller.GetSample());
    ASSERT_DOUBLE_EQ(0, controller.GetSample());
    ASSERT_DOUBLE_EQ(0.25, controller.GetSample());
  }

  // Silence
  env.NoteOff();
  for (int i = 0; i < 10; i++) {
    ASSERT_DOUBLE_EQ(0.0, controller.GetSample());
  }
}

}  // namespace

int main(int argc, char* argv[]) {
  TestController();
  std::cout << "PASS" << std::endl;
  return 0;
}
