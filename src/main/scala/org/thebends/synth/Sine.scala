package org.thebends.synth

import java.lang.Math;

class Sine(frequency: Generator) extends Generator {
  val FREQUENCY:Float = 440.0f  // Hz
  var sample: Long = 0;

  def generate(): Double = {
    var freq = frequency.generate()
    if (freq < 0.01f) {
      return 0.0f;
    }
    val periodSamples: Long = (Environment.SAMPLE_RATE / freq).toLong;
    sample = (sample + 1) % periodSamples;
    val x = sample / periodSamples.toDouble;
    return Math.sin(2.0f * Math.PI * x)
  }
}
