package org.thebends.synth

import java.lang.Math;
import javax.sound.sampled.AudioFormat
import javax.sound.sampled.AudioSystem
import javax.sound.sampled.DataLine
import javax.sound.sampled.SourceDataLine

import ValueImplicits._;


/**
 * Simple program to play a wave, as a first timer exercise of PCM
 * synthesis in Scala.
 */
object Player extends Application {
  val RATE:Double = 5.0f  // Hz
  val FREQUENCY:Double = 550.0f  // Hz
  val BUFFER_SIZE = 1024;
  val lfo = new Oscillator(RATE, Oscillators.sawtooth);
  var freq = new ControlValue() {
      def value(): Double = {
        return FREQUENCY + FREQUENCY * lfo.value()
      }
    }
  val osc = new Oscillator(freq, Oscillators.sawtooth);
  println("Playing...(from Scala)")
  val amp = new Output(osc, BUFFER_SIZE)
  amp.start
}
