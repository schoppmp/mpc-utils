#include "benchmarker.hpp"
#include <thread>
#include "gtest/gtest.h"

TEST(Benchmarker, AddTimeSinceStart) {
  Benchmarker b;
  auto start = b.StartTimer();
  std::this_thread::sleep_for(std::chrono::seconds(1));
  b.AddSecondsSinceStart("sleep", start);
  EXPECT_GT(b.GetTotal("sleep"), 0.9);
}

TEST(Benchmarker, AddAmount) {
  Benchmarker b;
  b.AddAmount("amount", 5);
  EXPECT_EQ(b.GetTotal("amount"), 5);
}
