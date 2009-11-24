package org.thebends.synth;

import junit.framework.TestCase;

public class EnvelopeTest extends TestCase {
  private static final double TOLERANCE = 0.00001;

  public void testFlat() throws Exception {
    Envelope env = new Envelope();
    env.noteOn();
    for (int i = 0; i < 10; ++i) {
      assertEquals(1.0, env.getValue(), TOLERANCE);
    }
    env.noteOff();
    for (int i = 0; i < 10; ++i) {
      assertEquals(0.0, env.getValue(), TOLERANCE);
    }
  }

  public void testZero() throws Exception {
    Envelope env = new Envelope();
    env.setAttack(0);
    env.setDecay(0);
    env.setSustain(0);
    env.setRelease(0);
    env.noteOn();
    for (int i = 0; i < 10; ++i) {
      assertEquals(0.0, env.getValue(), TOLERANCE);
    }
    env.noteOff();
    for (int i = 0; i < 10; ++i) {
      assertEquals(0.0, env.getValue(), TOLERANCE);
    }
  }

  public void testCurve() throws Exception {
    Envelope env = new Envelope();
    env.setAttack(4);
    env.setDecay(4);
    env.setSustain(0.45);
    env.setRelease(8);
    env.noteOn();
    // Attack
    assertEquals(0.25, env.getValue(), TOLERANCE);
    assertEquals(0.5, env.getValue(), TOLERANCE);
    assertEquals(0.75, env.getValue(), TOLERANCE);
    assertEquals(1.0, env.getValue(), TOLERANCE);
    // Decay
    assertEquals(0.8625, env.getValue(), TOLERANCE);
    assertEquals(0.725, env.getValue(), TOLERANCE);
    assertEquals(0.5875, env.getValue(), TOLERANCE);
    // Sustain
    for (int i = 0; i < 10; ++i) {
      assertEquals(0.45, env.getValue(), TOLERANCE);
    }
    env.noteOff();
    // Release
    assertEquals(0.39375, env.getValue(), TOLERANCE);
    assertEquals(0.3375, env.getValue(), TOLERANCE);
    assertEquals(0.28125, env.getValue(), TOLERANCE);
    assertEquals(0.225, env.getValue(), TOLERANCE);
    assertEquals(0.16875, env.getValue(), TOLERANCE);
    assertEquals(0.1125, env.getValue(), TOLERANCE);
    assertEquals(0.05625, env.getValue(), TOLERANCE);
    // Done
    for (int i = 0; i < 10; ++i) {
      assertEquals(0.0, env.getValue(), TOLERANCE);
    }
  }

  public void testAttackRelease() throws Exception {
    Envelope env = new Envelope();
    env.setAttack(4);
    env.setDecay(0);
    env.setSustain(0.99);
    env.setRelease(3);
    env.noteOn();
    // Attack
    assertEquals(0.25, env.getValue(), TOLERANCE);
    assertEquals(0.5, env.getValue(), TOLERANCE);
    assertEquals(0.75, env.getValue(), TOLERANCE);
    env.noteOff();
    // Release before we finished attacking.  Release from current value
    // Decay
    assertEquals(0.5, env.getValue(), TOLERANCE);
    assertEquals(0.25, env.getValue(), TOLERANCE);
    for (int i = 0; i < 10; ++i) {
      assertEquals(0.0, env.getValue(), TOLERANCE);
    }
  }

  public void testDecay() throws Exception {
    Envelope env = new Envelope();
    env.setAttack(0);
    env.setDecay(5);
    env.setSustain(0.0); 
    env.setRelease(8);
    env.noteOn();
    assertEquals(0.8, env.getValue(), TOLERANCE);
    assertEquals(0.6, env.getValue(), TOLERANCE);
    assertEquals(0.4, env.getValue(), TOLERANCE);
    assertEquals(0.2, env.getValue(), TOLERANCE);
    for (int i = 0; i < 10; ++i) {
      assertEquals(0.0, env.getValue(), TOLERANCE);
    }
    env.noteOff();
    for (int i = 0; i < 10; ++i) {
      assertEquals(0.0, env.getValue(), TOLERANCE);
    }
  }

  public void testDecayRelease() throws Exception {
    Envelope env = new Envelope();
    env.setAttack(4);
    env.setDecay(4);
    env.setSustain(0.5); 
    env.setRelease(8);
    env.noteOn();
    // Attack
    assertEquals(0.25, env.getValue(), TOLERANCE);
    assertEquals(0.5, env.getValue(), TOLERANCE);
    assertEquals(0.75, env.getValue(), TOLERANCE);
    assertEquals(1.0, env.getValue(), TOLERANCE);
    // Decay
    assertEquals(0.875, env.getValue(), TOLERANCE);
    assertEquals(0.75, env.getValue(), TOLERANCE);
    env.noteOff();
    // Released before we finished decaying.  Release starts from the decay
    // point.
    assertEquals(0.65625, env.getValue(), TOLERANCE);
    assertEquals(0.5625, env.getValue(), TOLERANCE);
    assertEquals(0.46875, env.getValue(), TOLERANCE);
    assertEquals(0.375, env.getValue(), TOLERANCE);
    assertEquals(0.28125, env.getValue(), TOLERANCE);
    assertEquals(0.1875, env.getValue(), TOLERANCE);
    assertEquals(0.09375, env.getValue(), TOLERANCE);
    for (int i = 0; i < 10; ++i) {
      assertEquals(0.0, env.getValue(), TOLERANCE);
    }
  }

}
