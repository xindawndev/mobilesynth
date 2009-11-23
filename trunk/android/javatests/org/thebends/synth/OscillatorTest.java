package org.thebends.synth;

import junit.framework.TestCase;

public class OscillatorTest extends TestCase {
  private static final double TOLERANCE = 0.00001;

  public void testSine() throws Exception {
    Oscillator osc = new Oscillator(FixedParameter.get(1.0), 4.0f);
    osc.setWaveForm(Oscillator.WaveForm.SINE);

    for (int i = 0; i < 10; ++i) {
      assertEquals(0.0, osc.getValue(), TOLERANCE);
      assertEquals(1.0, osc.getValue(), TOLERANCE);
      assertEquals(0.0, osc.getValue(), TOLERANCE);
      assertEquals(-1.0, osc.getValue(), TOLERANCE);
    }
  }

  public void testSquare() throws Exception {
    Oscillator osc = new Oscillator(FixedParameter.get(1.0), 7.0f);
    osc.setWaveForm(Oscillator.WaveForm.SQUARE);

    for (int i = 0; i < 10; ++i) {
      assertEquals(1.0, osc.getValue(), TOLERANCE);
      assertEquals(1.0, osc.getValue(), TOLERANCE);
      assertEquals(1.0, osc.getValue(), TOLERANCE);
      assertEquals(-1.0, osc.getValue(), TOLERANCE);
      assertEquals(-1.0, osc.getValue(), TOLERANCE);
      assertEquals(-1.0, osc.getValue(), TOLERANCE);
      assertEquals(-1.0, osc.getValue(), TOLERANCE);
    }
  }

  public void testSawtooth() throws Exception {
    Oscillator osc = new Oscillator(FixedParameter.get(1.0), 8.0f);
    osc.setWaveForm(Oscillator.WaveForm.SAWTOOTH);

    for (int i = 0; i < 10; ++i) {
      assertEquals(-1.0, osc.getValue(), TOLERANCE);
      assertEquals(-0.75, osc.getValue(), TOLERANCE);
      assertEquals(-0.5, osc.getValue(), TOLERANCE);
      assertEquals(-0.25, osc.getValue(), TOLERANCE);
      assertEquals(0.0, osc.getValue(), TOLERANCE);
      assertEquals(0.25, osc.getValue(), TOLERANCE);
      assertEquals(0.5, osc.getValue(), TOLERANCE);
      assertEquals(0.75, osc.getValue(), TOLERANCE);
    }
  }

  public void testSawtooth2() throws Exception {
    Oscillator osc = new Oscillator(FixedParameter.get(2.0), 8.0f);
    osc.setWaveForm(Oscillator.WaveForm.SAWTOOTH);

    for (int i = 0; i < 10; ++i) {
      assertEquals(-1.0, osc.getValue(), TOLERANCE);
      assertEquals(-0.5, osc.getValue(), TOLERANCE);
      assertEquals(0.0, osc.getValue(), TOLERANCE);
      assertEquals(0.5, osc.getValue(), TOLERANCE);
    }
  }

  public void testReverseSawtooth() throws Exception {
    Oscillator osc = new Oscillator(FixedParameter.get(1.0), 8.0f);
    osc.setWaveForm(Oscillator.WaveForm.REVERSE_SAWTOOTH);

    for (int i = 0; i < 10; ++i) {
      assertEquals(1.0, osc.getValue(), TOLERANCE);
      assertEquals(0.75, osc.getValue(), TOLERANCE);
      assertEquals(0.5, osc.getValue(), TOLERANCE);
      assertEquals(0.25, osc.getValue(), TOLERANCE);
      assertEquals(0.0, osc.getValue(), TOLERANCE);
      assertEquals(-0.25, osc.getValue(), TOLERANCE);
      assertEquals(-0.5, osc.getValue(), TOLERANCE);
      assertEquals(-0.75, osc.getValue(), TOLERANCE);
    }
  }

  public void testTriangle() throws Exception {
    Oscillator osc = new Oscillator(FixedParameter.get(1.0), 8.0f);
    osc.setWaveForm(Oscillator.WaveForm.TRIANGLE);

    for (int i = 0; i < 10; ++i) {
      assertEquals(1.0, osc.getValue(), TOLERANCE);
      assertEquals(0.5, osc.getValue(), TOLERANCE);
      assertEquals(0.0, osc.getValue(), TOLERANCE);
      assertEquals(-0.5, osc.getValue(), TOLERANCE);
      assertEquals(-1.0, osc.getValue(), TOLERANCE);
      assertEquals(-0.5, osc.getValue(), TOLERANCE);
      assertEquals(0.0, osc.getValue(), TOLERANCE);
      assertEquals(0.5, osc.getValue(), TOLERANCE);
    }
  }

  public void testTriangle2() throws Exception {
    Oscillator osc = new Oscillator(FixedParameter.get(2.0), 8.0f);
    osc.setWaveForm(Oscillator.WaveForm.TRIANGLE);

    for (int i = 0; i < 10; ++i) {
      assertEquals(1.0, osc.getValue(), TOLERANCE);
      assertEquals(0.0, osc.getValue(), TOLERANCE);
      assertEquals(-1.0, osc.getValue(), TOLERANCE);
      assertEquals(0.0, osc.getValue(), TOLERANCE);
    }
  }

}
