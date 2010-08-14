// numeric_test.cpp
// Author: Allen Porter <allen@thebends.org>

#include <glog/logging.h>
#include <gtest/gtest.h>
#include "numeric.h"

using namespace ::ysynth;

namespace {

TEST(Counter, BinaryNoRepeat) {
  Counter counter(2, NONE);
  EXPECT_FLOAT_EQ(0.0f, counter.GetValue());
  for (int i = 0; i < 10; i++) {
    EXPECT_FLOAT_EQ(1.0f, counter.GetValue());
  }
}

TEST(Counter, NoRepeat) {
  Counter counter(3, NONE);
  EXPECT_FLOAT_EQ(0.0f, counter.GetValue());
  EXPECT_FLOAT_EQ(0.5f, counter.GetValue());
  for (int i = 0; i < 10; i++) {
    EXPECT_FLOAT_EQ(1.0f, counter.GetValue());
  }
}

TEST(Counter, BinaryLoop) {
  Counter counter(2, LOOP);
  for (int i = 0; i < 10; i++) {
    EXPECT_FLOAT_EQ(0.0f, counter.GetValue());
    EXPECT_FLOAT_EQ(1.0f, counter.GetValue());
  }
}

TEST(Counter, Loop) {
  Counter counter(3, LOOP);
  for (int i = 0; i < 10; i++) {
    EXPECT_FLOAT_EQ(0.0f, counter.GetValue());
    EXPECT_FLOAT_EQ(0.5f, counter.GetValue());
    EXPECT_FLOAT_EQ(1.0f, counter.GetValue());
  }
}

TEST(Counter, BinaryMirroredLoop) {
  Counter counter(2, MIRRORED_LOOP);
  for (int i = 0; i < 10; i++) {
    EXPECT_FLOAT_EQ(0.0f, counter.GetValue());
    EXPECT_FLOAT_EQ(1.0f, counter.GetValue());
  }
}

TEST(Counter, MirroredLoop) {
  Counter counter(3, MIRRORED_LOOP);
  for (int i = 0; i < 10; i++) {
    EXPECT_FLOAT_EQ(0.0f, counter.GetValue());
    EXPECT_FLOAT_EQ(0.5f, counter.GetValue());
    EXPECT_FLOAT_EQ(1.0f, counter.GetValue());
    EXPECT_FLOAT_EQ(0.5f, counter.GetValue());
  }
}

TEST(Interpolation, NoRepeat) {
  Interpolation<long> interp(1, 5, 5, NONE);
  EXPECT_FLOAT_EQ(1.0f, interp.GetValue());
  EXPECT_FLOAT_EQ(2.0f, interp.GetValue());
  EXPECT_FLOAT_EQ(3.0f, interp.GetValue());
  EXPECT_FLOAT_EQ(4.0f, interp.GetValue());
  for (int i = 0; i < 10; i++) {
    EXPECT_FLOAT_EQ(5.0f, interp.GetValue());
  }
}

TEST(Interpolation, Loop) {
  Interpolation<long> interp(1, 5, 5, LOOP);
  for (int i = 0; i < 10; i++) {
    EXPECT_FLOAT_EQ(1.0f, interp.GetValue());
    EXPECT_FLOAT_EQ(2.0f, interp.GetValue());
    EXPECT_FLOAT_EQ(3.0f, interp.GetValue());
    EXPECT_FLOAT_EQ(4.0f, interp.GetValue());
    EXPECT_FLOAT_EQ(5.0f, interp.GetValue());
  }
}

TEST(Interpolation, MirroredLoop) {
  Interpolation<long> interp(1, 5, 5, MIRRORED_LOOP);
  for (int i = 0; i < 10; i++) {
    EXPECT_FLOAT_EQ(1.0f, interp.GetValue());
    EXPECT_FLOAT_EQ(2.0f, interp.GetValue());
    EXPECT_FLOAT_EQ(3.0f, interp.GetValue());
    EXPECT_FLOAT_EQ(4.0f, interp.GetValue());
    EXPECT_FLOAT_EQ(5.0f, interp.GetValue());
    EXPECT_FLOAT_EQ(4.0f, interp.GetValue());
    EXPECT_FLOAT_EQ(3.0f, interp.GetValue());
    EXPECT_FLOAT_EQ(2.0f, interp.GetValue());
  }
}

}  // namespace

int main(int argc, char* argv[]) {
  google::InstallFailureSignalHandler();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
