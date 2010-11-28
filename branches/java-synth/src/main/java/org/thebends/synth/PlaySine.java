package org.thebends.synth;

import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.DataLine;
import javax.sound.sampled.SourceDataLine;

/**
 * Simple program to play a sine wave, as a first timer exercise of PCM
 * synthesis in Java.
 */
public class PlaySine {
  private static final float SAMPLE_RATE = 44100.0f;
  private static final int SAMPLE_SIZE = 8;  // bits
  private static final AudioFormat FORMAT = new AudioFormat(
      SAMPLE_RATE, SAMPLE_SIZE, /* mono */ 1, /* signed */ true,
      /* bigEndian */ true);
  private static final int FREQUENCY = 440;  // Hz
  private static final int PERIOD_SAMPLES = (int) (SAMPLE_RATE / FREQUENCY);

  public static void main(String[] args) throws Exception {
    // The buffer is a even multiple of the number of samples for a single
    // period to make things a little simpler.
    int bufferSize = PERIOD_SAMPLES * 1024;
    byte[] buffer = new byte[bufferSize];
    for (int i = 0; i < bufferSize; ++i) {
      double x = (i % PERIOD_SAMPLES) / (double) PERIOD_SAMPLES;
      double value = Math.sin(2.0f * Math.PI * x);
      // Since this is 8 bit audio we want a value between -127 and 127.
      buffer[i] = (byte) (value * 127);
    }

    System.err.println("Playing...");
    SourceDataLine line = (SourceDataLine) AudioSystem.getLine(
        new DataLine.Info(SourceDataLine.class, FORMAT));
    line.open(FORMAT);
    line.start();
    while (true) {
      line.write(buffer, 0, bufferSize);
    }
  }
}
