// gate.cpp
// Author: Allen Porter <allen@thebends.org>

#include "gate.h"

#include <sys/param.h>

namespace ysynth {

class ConstGate : public Gate {
 public:
  ConstGate(bool value) : value_(value) { }
  virtual ~ConstGate() { }

  virtual bool GetValue() { return value_; }

 private:
  bool value_;
};

Gate* open_gate = NULL;

Gate* OpenGate() {
  if (open_gate == NULL) {
    open_gate = new ConstGate(true);
  }
  return open_gate;
}

Gate* closed_gate(NULL);

Gate* ClosedGate() {
  if (closed_gate == NULL) {
    closed_gate = new ConstGate(false);
  }
  return closed_gate;
}


}  // namespace ysynth
