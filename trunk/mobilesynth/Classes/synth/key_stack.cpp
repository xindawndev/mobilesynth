// key_stack.cpp
// Author: Allen Porter <allen@thebends.org>

#include "key_stack.h"

using namespace std;

namespace synth {

bool KeyStack::NoteOn(int note) {
  for (vector<int>::iterator it = notes_.begin(); it != notes_.end(); ++it) {
    if (*it == note) {
      // Note already on the stack, nothing to do
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
  return false;
}

int KeyStack::GetCurrentNote() {
  if (notes_.size() > 0) {
    return notes_.back();
  } else {
    return 0;
  }
}

}  // namespace synth
