package org.thebends.mobilesynth;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;

import org.thebends.synth.Parameter;

class SynthTrack {

  private static final int SAMPLE_RATE_HZ = 44100;
  private static final int MIN_BUFFER_SIZE =
      AudioTrack.getMinBufferSize(SAMPLE_RATE_HZ, AudioFormat.CHANNEL_OUT_MONO,
          AudioFormat.ENCODING_PCM_16BIT);
  private static final int BUFFER_SIZE = Math.max(MIN_BUFFER_SIZE, 1024);

  private final AudioTrack track;
  private final Thread thread;

  public SynthTrack(Parameter parameter) {
    track = new AudioTrack(AudioManager.STREAM_MUSIC, SAMPLE_RATE_HZ,
        AudioFormat.CHANNEL_OUT_MONO, AudioFormat.ENCODING_PCM_16BIT,
        BUFFER_SIZE, AudioTrack.MODE_STREAM);
    track.play();
    thread = new Thread(new SynthRunner(parameter, track), "SynthTrack");
    thread.start();
  }

  private static class SynthRunner implements Runnable {
    private final Parameter parameter;
    private final AudioTrack track;

    public SynthRunner(Parameter parameter, AudioTrack track) {
      this.parameter = parameter;
      this.track = track;
    }

    @Override
    public void run() {
System.err.println("Thread started");
      while (true) {
        short[] data = getBuffer();
        track.write(data, 0, BUFFER_SIZE);
      }
    }

    /**
     * Fill an entire buffer.
     */
    private short[] getBuffer() {
      short buffer[] = new short[BUFFER_SIZE];
      for (int i = 0; i < BUFFER_SIZE; ++i) {
        buffer[i] = getShortValue(parameter.getValue());
      }
      return buffer;
    }

    /**
     * Converts a linear PCM value from a double to a short.
     */
    private static short getShortValue(double value) {
      return (short) (value * Short.MAX_VALUE);
    }
  };
}
