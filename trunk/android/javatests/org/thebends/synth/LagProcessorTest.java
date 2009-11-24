package org.thebends.synth;

import junit.framework.TestCase;

public class LagProcessorTest extends TestCase {
  private static final double TOLERANCE = 0.00001;

  public void testFlat() throws Exception {
    MutableParameter param = new MutableParameter();
    param.setValue(1.0);
    LagProcessor glide = new LagProcessor(param);
    for (int i = 0; i < 10; ++i) {
      assertEquals(1.0, glide.getValue(), TOLERANCE);
    }
    // Rate has no effect when nothing changes
    glide.setSamples(10);
    for (int i = 0; i < 10; ++i) {
      assertEquals(1.0, glide.getValue(), TOLERANCE);
    }
  }

  public void testUpDown() throws Exception {
    MutableParameter param = new MutableParameter();
    param.setValue(0.0);
    LagProcessor glide = new LagProcessor(param);
    glide.setSamplesUp(4);
    glide.setSamplesDown(10);
    for (int i = 0; i < 10; ++i) {
      assertEquals(0.0, glide.getValue(), TOLERANCE);
    }
    // Walk up over 5 samples
    param.setValue(2.0);
    assertEquals(0.25, glide.getValue(), TOLERANCE);
    assertEquals(0.5, glide.getValue(), TOLERANCE);
    assertEquals(0.75, glide.getValue(), TOLERANCE);
    assertEquals(1.0, glide.getValue(), TOLERANCE);
    assertEquals(1.25, glide.getValue(), TOLERANCE);
    assertEquals(1.5, glide.getValue(), TOLERANCE);
    assertEquals(1.75, glide.getValue(), TOLERANCE);
    for (int i = 0; i < 10; ++i) {
      assertEquals(2.0, glide.getValue(), TOLERANCE);
    }
    param.setValue(1.0);
    glide.setSamplesDown(0);
    for (int i = 0; i < 10; ++i) {
      assertEquals(1.0, glide.getValue(), TOLERANCE);
    }
    // Walk down over 10 samples
    param.setValue(0.0);
    glide.setSamplesDown(10);
    assertEquals(0.9, glide.getValue(), TOLERANCE);
    assertEquals(0.8, glide.getValue(), TOLERANCE);
    assertEquals(0.7, glide.getValue(), TOLERANCE);
    assertEquals(0.6, glide.getValue(), TOLERANCE);
    assertEquals(0.5, glide.getValue(), TOLERANCE);
    assertEquals(0.4, glide.getValue(), TOLERANCE);
    assertEquals(0.3, glide.getValue(), TOLERANCE);
    assertEquals(0.2, glide.getValue(), TOLERANCE);
    assertEquals(0.1, glide.getValue(), TOLERANCE);
    for (int i = 0; i < 10; ++i) {
      assertEquals(0.0, glide.getValue(), TOLERANCE);
    }
  }

  public void testInterrupt() throws Exception {
    MutableParameter param = new MutableParameter();
    param.setValue(0.0);
    LagProcessor glide = new LagProcessor(param);
    glide.setSamplesUp(5);
    glide.setSamplesDown(4);
    for (int i = 0; i < 10; ++i) {
      assertEquals(0.0, glide.getValue(), TOLERANCE);
    }
    // Walk up over 5 samples, but interrupt in the middle
    param.setValue(1.0);
    assertEquals(0.2, glide.getValue(), TOLERANCE);
    assertEquals(0.4, glide.getValue(), TOLERANCE);
    param.setValue(0.0);
    assertEquals(0.3, glide.getValue(), TOLERANCE);
    assertEquals(0.2, glide.getValue(), TOLERANCE);
    assertEquals(0.1, glide.getValue(), TOLERANCE);
    for (int i = 0; i < 10; ++i) {
      assertEquals(0.0, glide.getValue(), TOLERANCE);
    }
  }

  public void testImmediate() throws Exception {
    MutableParameter param = new MutableParameter();
    param.setValue(0.0);
    LagProcessor glide = new LagProcessor(param);
    glide.setSamples(0);
    for (int i = 0; i < 10; ++i) {
      assertEquals(0.0, glide.getValue(), TOLERANCE);
    }
    param.setValue(1.0);
    for (int i = 0; i < 10; ++i) {
      assertEquals(1.0, glide.getValue(), TOLERANCE);
    }
  }

  public void testReset() throws Exception {
    MutableParameter param = new MutableParameter();
    param.setValue(0.0);
    LagProcessor glide = new LagProcessor(param);
    glide.setSamples(2);
    for (int i = 0; i < 10; ++i) {
      assertEquals(0.0, glide.getValue(), TOLERANCE);
    }
    // No glide after reset
    glide.reset();
    param.setValue(1.0);
    for (int i = 0; i < 10; ++i) {
      assertEquals(1.0, glide.getValue(), TOLERANCE);
    }
    param.setValue(2.0);
    assertEquals(1.5, glide.getValue(), TOLERANCE);
    for (int i = 0; i < 10; ++i) {
      assertEquals(2.0, glide.getValue(), TOLERANCE);
    }
    // No glide after reset
    glide.reset();
    param.setValue(3.0);
    for (int i = 0; i < 10; ++i) {
      assertEquals(3.0, glide.getValue(), TOLERANCE);
    }
    param.setValue(2.0);
    assertEquals(2.5, glide.getValue(), TOLERANCE);
    for (int i = 0; i < 10; ++i) {
      assertEquals(2.0, glide.getValue(), TOLERANCE);
    }
  }

}
