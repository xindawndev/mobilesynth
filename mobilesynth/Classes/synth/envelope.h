// envelope.h
// Author: Allen Porter <allen@thebends.org>
//
// An envelope controls a value over time.

#ifndef __ENVELOPE_H__
#define __ENVELOPE_H__

namespace synth {

template<class T>
class Envelope {
 public:
  Envelope() : attack_(0),
               decay_(0),
               decay_end_(0),
               sustain_(0),
               release_(0),
               release_start_(0),
               release_end_(0),
               min_(0),
               max_(0),
               current_(0),
               state_(DONE) { }

  // samples
  void set_attack(long attack) { attack_ = attack; }

  // samples
  void set_decay(long decay) { decay_ = decay; }

  // Sustain Volumne
  void set_sustain(T sustain) { sustain_ = sustain; }

  // samples
  void set_release(long release) { release_ = release; }

  // The value reached at the peak of the attack (Typically 1.0).
  void set_max(T max) { max_ = max; }
  // The value at the start and release (Typically 0.0).
  void set_min(T min) { min_ = min; }

  // Invoked when the note is pressed, resets all counters.
  void NoteOn() {
    assert(min_ != max_);
    current_ = 0;
    decay_end_ = attack_ + decay_;
    if (attack_ == 0) {
      attack_slope_ = 1;
    } else {
      // Fixed point division with an integer
      attack_slope_ = (max_ - min_) / attack_;
    }
    if (decay_ == 0) {
      decay_slope_ = 1;
    } else {
      // Fixed point division with an integer
      decay_slope_ = (max_ - sustain_) / decay_;
    }
    state_ = ATTACK;
  }

  // Invoked when the note is released.
  void NoteOff() {
    state_ = RELEASE;
    release_start_value_ = last_value_;
    if (release_ == 0) {
      release_slope_ = 1;
    } else {
      release_slope_ = (release_start_value_ - min_) / release_;
    }
    release_start_ = current_;
    release_end_ = current_ + release_;
  }

  // Advances the clock and returns the value for the current step.  Should not
  // be called when Done() returns false.
  T GetValue() {
    current_++;
    T value = 0;
    // Check that we haven't transitioned longo the next state
    if (state_ == ATTACK || state_ == DECAY) {
      if (current_ > decay_end_) {
        state_ = SUSTAIN;
      } else if (current_ > attack_) {
        state_ = DECAY;
      }
    }
    if (state_ == SUSTAIN) {
      if (sustain_ <= 0.0) {
        state_ = DONE;
      }
    }
    if (state_ == RELEASE) {
      if (current_ > release_end_) {
        state_ = DONE;
      }
    }
    switch (state_) {
      case ATTACK:
        // current (integer) * attack_slope_ (fixed point)
        value = current_ * attack_slope_ + min_;
        value = std::min(value, max_);
        break;
      case DECAY:
        assert(current_ >= attack_);
        // integer * fixed point
        value = max_ - (current_ - attack_) * decay_slope_;
        value = std::max(value, sustain_);
        break;
      case SUSTAIN:
        value = sustain_;
        assert(value > 0.0);  // Handled in DECAY
        break;
      case RELEASE:
        //assert(current_ >= release_start_);
        // integer * fixed point
        value = release_start_value_ -
          (current_ - release_start_) * release_slope_;
        value = std::max(value, min_);
        break;
      case DONE:
        value = min_;
        break;
      default:
        assert(false);
        break;
    }
    last_value_ = value;
    return value;
  }


  // True when the note has finished playing.
  bool released() const { return (state_ == DONE); }

 private:
  enum State {
    ATTACK = 0,
    DECAY = 1,
    SUSTAIN = 2,
    RELEASE = 3,
    DONE = 4,
  };
  long attack_;
  T attack_slope_;  // T
  long decay_;
  long decay_end_;
  T decay_slope_;  // T
  T sustain_;  // T
  long release_;
  long release_start_;
  long release_end_;
  T release_slope_;  // T

  T min_;  // T
  T max_;  // T

  long current_;  // sample
  T last_value_;  // T
  State state_;
  T release_start_value_;  // T
};

}  // namespace synth

#endif  // __ENVELOPE_H__
