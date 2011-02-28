package org.thebends.synth

import javax.sound.sampled.AudioFormat
import javax.sound.sampled.AudioSystem
import javax.sound.sampled.DataLine
import javax.sound.sampled.SourceDataLine

import scala.actors.Actor
import scala.actors.Actor._

class Amplifier(input: Generator,
                bufferSize: Int) extends Actor {
  val FORMAT = new AudioFormat(
      Environment.SAMPLE_RATE,
      Environment.SAMPLE_SIZE,
      /* mono */ 1, /* signed */ true,
      /* bigEndian */ true)

  def toByte(in: Double): Byte = {
    // Since this is 8 bit audio we want a value between -127 and 127.
    return (in.toInt * 127).toByte
  }

  def act() {
    val l = AudioSystem.getLine(
        new DataLine.Info(classOf[SourceDataLine], FORMAT))
    val line = l.asInstanceOf[SourceDataLine]
    line.open(FORMAT)
    line.start()
    while (true) {
      var buffer = List.fill(bufferSize)(input.generate)
        .map(toByte)
        .toArray;
      line.write(buffer, 0, bufferSize);
    }
  }
}
