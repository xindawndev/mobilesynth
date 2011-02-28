package org.thebends.synth

trait Generator {
  def generate(): Double
}

class ConstGenerator(value: Double) extends Generator {
  def generate(): Double = value
}

object GeneratorImplicits {
  implicit def floatToGenerator(value: Double) = new ConstGenerator(value)
}
import GeneratorImplicits._;
