package org.thebends.synth

trait AudioValue {
  def value(): Double
}

trait ControlValue {
  def value(): Double
}

trait Gate {
  def open(): Boolean
}

class GatedControlValue(in: ControlValue, trigger: Gate) extends ControlValue {
  def value(): Double = {
    val value = in.value();
    if (trigger.open()) {
      return value
    }
    return 0
  }
}

object ValueImplicits {
  implicit def floatToAudioValue(x: Double) = new AudioValue {
    def value(): Double = x
  }
  implicit def floatToControlValue(x: Double) = new ControlValue {
    def value(): Double = x
  }
}
