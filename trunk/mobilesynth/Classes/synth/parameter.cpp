// parameter.cpp
// Author: Allen Porter <allen@thebends.org>

#include "synth/parameter.h"
#include <math.h>

namespace synth {

FixedParameter::FixedParameter(float value) : value_(value) { }

FixedParameter::~FixedParameter() { }

float FixedParameter::GetValue() {
  return value_;
}

MutableParameter::MutableParameter(float value) : value_(value) { }

MutableParameter::~MutableParameter() { }

float MutableParameter::GetValue() {
  return value_;
}

void MutableParameter::set_value(float value) {
  value_ = value;
}

SumParameter::SumParameter() { }

SumParameter::~SumParameter() { }

float SumParameter::GetValue() {
  float value = 0;
  for (size_t i = 0; i < parameters_.size(); ++i) {
    value += parameters_[i]->GetValue();
  }
  return value;
}

void SumParameter::Clear() {
  parameters_.clear();
}

void SumParameter::AddParameter(Parameter* parameter) {
  parameters_.push_back(parameter);
}

MultiplyParameter::MultiplyParameter() { }

MultiplyParameter::~MultiplyParameter() { }

float MultiplyParameter::GetValue() {
  float value = (parameters_.size() > 0) ? 1.0 : 0.0;
  for (size_t i = 0; i < parameters_.size(); ++i) {
    value *= parameters_[i]->GetValue();
  }
  return value;
}

void MultiplyParameter::Clear() {
  parameters_.clear();
}


void MultiplyParameter::AddParameter(Parameter* parameter) {
  parameters_.push_back(parameter);
}

}  // namespace synth
