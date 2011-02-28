package org.thebends.synth

import java.lang.Math;
import javax.sound.sampled.AudioFormat
import javax.sound.sampled.AudioSystem
import javax.sound.sampled.DataLine
import javax.sound.sampled.SourceDataLine

/**
 * Simple program to play a sine wave, as a first timer exercise of PCM
 * synthesis in Scala.
 */
object Player extends Application {
  val FREQUENCY:Float = 440.0f  // Hz
  val BUFFER_SIZE = 1024;
  val sine = new Sine(new ConstGenerator(FREQUENCY));

  println("Playing...(from Scala)")
  val amp = new Amplifier(sine, BUFFER_SIZE)
  amp.start
}
