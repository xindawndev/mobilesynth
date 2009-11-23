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
}
