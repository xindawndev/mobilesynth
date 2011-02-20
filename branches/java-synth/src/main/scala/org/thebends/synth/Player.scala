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
  val SAMPLE_RATE:Float = 44100.0f
  val SAMPLE_SIZE = 8;  // bits
  val FORMAT = new AudioFormat(
      SAMPLE_RATE, SAMPLE_SIZE, /* mono */ 1, /* signed */ true,
      /* bigEndian */ true)
  val FREQUENCY:Float = 440.0f  // Hz
  val PERIOD_SAMPLES:Int = (SAMPLE_RATE / FREQUENCY).toInt

  // The buffer is a even multiple of the number of samples for a single
  // period to make things a little simpler.
  val bufferSize = PERIOD_SAMPLES * 1024
  var buffer:Array[Byte] = new Array[Byte](bufferSize)
  for (i <- 0 until bufferSize) {
    var x = (i % PERIOD_SAMPLES) / PERIOD_SAMPLES.toDouble
    var value = Math.sin(2.0f * Math.PI * x)
    // Since this is 8 bit audio we want a value between -127 and 127.
    buffer(i) = (value.toInt * 127).toByte
  }
  println("Playing...(from Scala)")
  var l = AudioSystem.getLine(
      new DataLine.Info(classOf[SourceDataLine], FORMAT))
  var line = l.asInstanceOf[SourceDataLine]
  line.open(FORMAT)
  line.start()
  while (true) {
    line.write(buffer, 0, bufferSize)
  }
}
