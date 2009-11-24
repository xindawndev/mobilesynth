package org.thebends.synth;

import junit.framework.TestCase;

public class LFOTest extends TestCase {
  private static final double TOLERANCE = 0.00001;

  public void testNoLevel() throws Exception {
    LFO mod = new LFO(FixedParameter.get(0.2), FixedParameter.get(0.0));
    for (int i = 0; i < 10; ++i) {
      assertEquals(1.0, mod.getValue(), TOLERANCE);
    }
  }

  public void testMaxLevel() throws Exception {
    MutableParameter osc = new MutableParameter();
    LFO mod = new LFO(osc, FixedParameter.get(1.0));

    osc.setValue(-1.0);
    assertEquals(0.0, mod.getValue(), TOLERANCE);
    osc.setValue(-0.5);
    assertEquals(0.25, mod.getValue(), TOLERANCE);
    osc.setValue(0.0);
    assertEquals(0.5, mod.getValue(), TOLERANCE);
    osc.setValue(0.5);
    assertEquals(0.75, mod.getValue(), TOLERANCE);
    osc.setValue(1.0);
    assertEquals(1.0, mod.getValue(), TOLERANCE);
  }

  public void testMidLevel() throws Exception {
    MutableParameter osc = new MutableParameter();
    LFO mod = new LFO(osc, FixedParameter.get(0.2));

    osc.setValue(-1.0);
    assertEquals(0.8, mod.getValue(), TOLERANCE);
    osc.setValue(-0.5);
    assertEquals(0.85, mod.getValue(), TOLERANCE);
    osc.setValue(0.0);
    assertEquals(0.9, mod.getValue(), TOLERANCE);
    osc.setValue(0.5);
    assertEquals(0.95, mod.getValue(), TOLERANCE);
    osc.setValue(1.0);
    assertEquals(1.0, mod.getValue(), TOLERANCE);
  }
}
