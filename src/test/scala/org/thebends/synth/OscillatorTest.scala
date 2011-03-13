package org.thebends.synth

import org.thebends.synth.Oscillators._
import org.thebends.synth.ValueImplicits._

import org.scalatest.FlatSpec
import org.scalatest.matchers.ShouldMatchers

class OscillatorTest extends FlatSpec with ShouldMatchers {

  "A Sawtooth Wave" should "be a sawtooth" in {
    Environment.SAMPLE_RATE = 8;
    var osc = new Oscillator(1.0, sawtooth);
    osc.value() should be(-0.75)
    osc.value() should be(-0.5)
    osc.value() should be(-0.25)
    osc.value() should be(0)
    osc.value() should be(0.25)
    osc.value() should be(0.5)
    osc.value() should be(0.75)
    osc.value() should be(-1)
    osc.value() should be(-0.75)
    osc.value() should be(-0.5)
    osc.value() should be(-0.25)
    osc.value() should be(0)
    osc.value() should be(0.25)
    osc.value() should be(0.5)
    osc.value() should be(0.75)
  }
  "A Square Wave" should "be a square" in {
    Environment.SAMPLE_RATE = 7;
    var osc = new Oscillator(1.0, square);
    osc.value() should be(1)
    osc.value() should be(1)
    osc.value() should be(1)
    osc.value() should be(-1)
    osc.value() should be(-1)
    osc.value() should be(-1)
    osc.value() should be(1)
    osc.value() should be(1)
    osc.value() should be(1)
    osc.value() should be(1)
    osc.value() should be(-1)
    osc.value() should be(-1)
    osc.value() should be(-1)
    osc.value() should be(1)
    osc.value() should be(1)
    osc.value() should be(1)
    osc.value() should be(1)
  }
}
