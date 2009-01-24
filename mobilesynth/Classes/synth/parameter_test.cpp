// parameter_test.cpp
// Author: Allen Porter <allen@thebends.org>

#include <assert.h>
#include <iostream>
#include "synth/parameter.h"
#include "synth/test_util.h"

namespace {

static void TestGetValue() {
  synth::Frequency freq;
  ASSERT_DOUBLE_EQ(1.0, freq.GetValue());

  freq.set_frequency(440.0);  // Middle A
  ASSERT_DOUBLE_EQ(440.0, freq.GetValue());

  // Set the offset to one octave up (1200 cents)
  freq.set_offset(1200);
  ASSERT_DOUBLE_EQ(880.0, freq.GetValue());

  // One octave down
  freq.set_offset(-1200);
  ASSERT_DOUBLE_EQ(220.0, freq.GetValue());

  // Down one note to G#
  freq.set_offset(-100);
  ASSERT_DOUBLE_EQ(415.305, freq.GetValue());

  // Change the note and the offset remains, resulting in one note down
  freq.set_frequency(415.305);
  ASSERT_DOUBLE_EQ(391.995, freq.GetValue());

  // Remove the offset
  freq.set_offset(0);
  ASSERT_DOUBLE_EQ(415.305, freq.GetValue());
}

static void TestNote() {
  synth::Note note;
  // Note A
  ASSERT_DOUBLE_EQ(440.0, note.GetValue());

  note.set_key(synth::Note::kMiddleAKey + 1);
  ASSERT_DOUBLE_EQ(466.164, note.GetValue());

  // Setting OCTAVE_2 plays A5
  note.set_key(synth::Note::kMiddleAKey);
  note.set_octave(synth::Note::OCTAVE_2);
  ASSERT_DOUBLE_EQ(880.0, note.GetValue());

  // A6
  note.set_key(synth::Note::kMiddleAKey);
  note.set_octave(synth::Note::OCTAVE_4);
  ASSERT_DOUBLE_EQ(1760.0, note.GetValue());

  // A7
  note.set_key(synth::Note::kMiddleAKey);
  note.set_octave(synth::Note::OCTAVE_8);
  ASSERT_DOUBLE_EQ(3520.0, note.GetValue());

  // A8
  note.set_key(synth::Note::kMiddleAKey);
  note.set_octave(synth::Note::OCTAVE_16);
  ASSERT_DOUBLE_EQ(7040.0, note.GetValue());

  // Back to A4
  note.set_key(synth::Note::kMiddleAKey);
  note.set_octave(synth::Note::NONE);
  ASSERT_DOUBLE_EQ(440.0, note.GetValue());
}

}  // namespace

int main(int argc, char* argv[]) {
  TestGetValue();
  TestNote();
  std::cout << "PASS" << std::endl;
  return 0;
}
