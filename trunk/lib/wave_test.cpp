// wave_test.cpp
// Author: Allen Porter <allen@thebends.org>

#include <glog/logging.h>
#include <gtest/gtest.h>
#include "wave.h"

using namespace ::ysynth;

namespace {

TEST(Wave, Sine) {
  Sine s;
  EXPECT_FLOAT_EQ(0.0f, s.GetValue(0.0f));
  EXPECT_FLOAT_EQ(1.0f, s.GetValue(0.25f));
  EXPECT_FLOAT_EQ(-8.7422777e-08, s.GetValue(0.5f));
  EXPECT_FLOAT_EQ(-1.0f, s.GetValue(0.75f));
  EXPECT_FLOAT_EQ(1.7484555e-07, s.GetValue(1.0f));
}

TEST(Wave, Square) {
  Square s;
  EXPECT_FLOAT_EQ(1.0f, s.GetValue(0.0f));
  EXPECT_FLOAT_EQ(1.0f, s.GetValue(0.1f));
  EXPECT_FLOAT_EQ(1.0f, s.GetValue(0.3f));
  EXPECT_FLOAT_EQ(1.0f, s.GetValue(0.4f));
  EXPECT_FLOAT_EQ(-1.0f, s.GetValue(0.5f));
  EXPECT_FLOAT_EQ(-1.0f, s.GetValue(0.6f));
  EXPECT_FLOAT_EQ(-1.0f, s.GetValue(0.7f));
  EXPECT_FLOAT_EQ(-1.0f, s.GetValue(0.9f));
  EXPECT_FLOAT_EQ(-1.0f, s.GetValue(1.0f));
}

TEST(Wave, Triangle) {
  Triangle s;
  EXPECT_FLOAT_EQ(1.0f, s.GetValue(0.0f));
  EXPECT_FLOAT_EQ(0.5f, s.GetValue(0.125f));
  EXPECT_FLOAT_EQ(0.0f, s.GetValue(0.25f));
  EXPECT_FLOAT_EQ(-0.5f, s.GetValue(0.375f));
  EXPECT_FLOAT_EQ(-1.0f, s.GetValue(0.5f));
  EXPECT_FLOAT_EQ(-0.5f, s.GetValue(0.625f));
  EXPECT_FLOAT_EQ(0.0f, s.GetValue(0.75f));
  EXPECT_FLOAT_EQ(0.5f, s.GetValue(0.875f));
  EXPECT_FLOAT_EQ(1.0f, s.GetValue(1.0f));
}

}  // namespace

int main(int argc, char* argv[]) {
  google::InstallFailureSignalHandler();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
