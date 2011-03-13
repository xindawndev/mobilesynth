package org.thebends.synth

import java.lang.Math

object Oscillators {

  def sin(x: Double): Double = {
    return Math.sin(2.0 * Math.PI * x)
  }

  def square(x: Double): Double = {
    if (x < 0.5) {
      return 1.0;
    }
    return -1.0;
  }

  def triangle(x: Double): Double = {
    return (2.0 * Math.abs(2.0f * x - 2.0f * Math.floor(x) - 1.0f) - 1.0f);
  }

  def sawtooth(x: Double): Double = {
    return 2.0 * (x - Math.floor(x) - 0.5);
  }

  def reverse_sawtooth(x: Double): Double = {
    return 2.0 * (Math.floor(x) - x + 0.5);
  }

}

class Oscillator(frequency: ControlValue,
                 f: (Double) => Double) extends AudioValue {
  var sample: Long = 0;

  def value(): Double = {
    var freq = frequency.value()
    val periodSamples: Long = (Environment.SAMPLE_RATE / freq).toLong;
    sample = (sample + 1) % periodSamples;
    val x = sample / periodSamples.toDouble;
    return f(x);
  }
}
