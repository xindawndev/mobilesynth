// key_stack.cpp
// Author: Allen Porter <allen@thebends.org>

#include "key_stack.h"
#include <iostream>
#include <math.h>

using namespace std;

namespace synth {

bool KeyStack::NoteOn(int note) {
  for (vector<int>::iterator it = notes_.begin(); it != notes_.end(); ++it) {
    if (*it == note) {
      // Insert a duplicate in the same position so does not override something
      // higher on the stack.
      notes_.insert(it, note);
      return false;
    }
  }
  notes_.push_back(note);
  return true;
}

bool KeyStack::NoteOff(int note) {
  for (vector<int>::iterator it = notes_.begin(); it != notes_.end(); ++it) {
    if (*it == note) {
      notes_.erase(it);
      return true;
    }
  }
  // The note wasn't on the stack.  The multi-touch events on the iphone seem
  // to be flaky, so we don't worry if we were asked to remove something that
  // was not on the stack.  The controller also calls our clear() method when
  // no touch events are left as a fallback. 
  return false;
}
  
bool KeyStack::IsNoteInStack(int note) {
  for (vector<int>::iterator it = notes_.begin(); it != notes_.end(); ++it) {
    if (*it == note) {
      return true;
    }
  }
  return false;
}

int KeyStack::GetCurrentNote() {
  if (notes_.size() > 0) {
    return notes_.back();
  } else {
    return 0;
  }
}

int KeyStack::GetNote(int num) {
  if (num > notes_.size()) {
    return 0;
  }
  return notes_[num];
}

static const int kMiddleAKey(49);
static const float kNotesPerOctave = 12.0f;
static const float kMiddleAFrequency = 440.0f;

float KeyToFrequency(int key) {
  return kMiddleAFrequency * powf(2, (key - kMiddleAKey) / kNotesPerOctave);
}

}  // namespace synth
