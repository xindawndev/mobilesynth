package org.thebends.synth

class Amplifier(gain: ControlValue, audio: AudioValue) extends AudioValue {
  def value(): Double = {
    return gain.value() * audio.value();
  }
}
