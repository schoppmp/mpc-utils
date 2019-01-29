#include "benchmarker.hpp"
#include <thread>
#include "gtest/gtest.h"

namespace mpc_utils {
namespace {

TEST(Benchmarker, AddTimeSinceStart) {
  Benchmarker b;
  auto start = b.StartTimer();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  b.AddSecondsSinceStart("sleep", start);
  EXPECT_GT(b.GetTotal("sleep"), 0.1);
}

TEST(Benchmarker, BenchmarkFunction) {
  Benchmarker b;
  b.BenchmarkFunction("sleep", [] {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
  });
  EXPECT_GT(b.GetTotal("sleep"), 0.1);
}

TEST(Benchmarker, MaybeBenchmarkFunction) {
  Benchmarker b;
  Benchmarker::MaybeBenchmarkFunction(&b, "sleep", [] {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
  });
  EXPECT_GT(b.GetTotal("sleep"), 0.1);
}

TEST(Benchmarker, MaybeBenchmarkFunctionNull) {
  Benchmarker b;
  auto start = b.StartTimer();
  Benchmarker::MaybeBenchmarkFunction(nullptr, "sleep_null", [] {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
  });
  b.AddSecondsSinceStart("sleep", start);
  EXPECT_EQ(b.GetTotal("sleep_null"), 0);
  EXPECT_GT(b.GetTotal("sleep"), 0.1);
}

TEST(Benchmarker, AddAmount) {
  Benchmarker b;
  b.AddAmount("amount", 5);
  EXPECT_EQ(b.GetTotal("amount"), 5);
}

TEST(Benchmarker, DefaultIsZero) {
  Benchmarker b;
  EXPECT_EQ(b.GetTotal("nothing"), 0);
}

}  // namespace
}  // namespace mpc_utils