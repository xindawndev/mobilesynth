// controller.h
// Author: Allen Porter <allen@thebends.org>
//
// The controller module returns samples and drives the oscillator and envelope.

#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <vector>

namespace synth {

class Envelope;
class LFO;
class Oscillator;

class Controller {
 public:
  Controller();
  
  // The envelope and oscillator must be set before generating samples.
  void add_oscillator(Oscillator* oscillator);
  void set_volume_envelope(Envelope* envelope);
  // TODO(allen): Filter envelope
  void set_lfo(LFO* lfo);

  void GetSamples(int num_output_samples, float* output_buffer);
  float GetSample();

 private:
  std::vector<Oscillator*> oscillators_;
  Envelope* volume_envelope_;
  LFO* lfo_;
};

}  // namespace synth

#endif // __CONTROLLER_H__
