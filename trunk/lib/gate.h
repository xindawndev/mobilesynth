// gate.h
// Author: Allen Porter <allen@thebends.org>

#include "types.h"

namespace ysynth {

class Gate : public Supplier<bool> {
 public:
  virtual ~Gate() { }

  virtual bool GetValue() = 0;

 protected:
  Gate() { }
};

Gate* OpenGate();
Gate* ClosedGate();

template<typename X>
class GatedSupplier : public Supplier<X> {
 public:
  GatedSupplier(Supplier<X>* in, Gate* trigger, X default_value)
      : in_(in), trigger_(trigger), default_value_(default_value) { }
  virtual ~GatedSupplier() { }

  virtual X GetValue() {
    X value = in_->GetValue();
    if (trigger_->GetValue()) {
      return value;
    }
    return default_value_;
  }

 private:
  Supplier<X>* in_;
  Gate* trigger_;
  X default_value_;
};

}  // namespace ysynth
