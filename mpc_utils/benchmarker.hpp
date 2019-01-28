// A Benchmarker can be used to keep track of timings and other measurements.
// It is especially useful for benchmarking interleaved phases of different
// computation types (e.g., local computation vs. network communication).
//
// Each measurement is associated with a key. For a given key, get(key) returns
// the sum of all measurements with that key.

#pragma once
#include <chrono>
#include <functional>
#include <unordered_map>

class Benchmarker {
 public:
  Benchmarker() {}

  // Benchmarker is not copyable or movable.
  Benchmarker(const Benchmarker& other) = delete;
  Benchmarker& operator=(const Benchmarker& other) = delete;

  Benchmarker(Benchmarker&& other) = delete;
  Benchmarker& operator=(Benchmarker&& other) = delete;

  using time_point = std::chrono::time_point<std::chrono::steady_clock>;

  // Starts a timer and returns a handle for it.
  time_point StartTimer() const;

  // Adds the number of seconds spent since start_time to the measurements with
  // the given key.
  void AddSecondsSinceStart(const std::string& key,
                            const time_point& start_time);

  // Runs f and adds the number of seconds it takes to the given key.
  void BenchmarkFunction(const std::string&, std::function<void()> f);

  // Adds a user-defined amount to the given key.
  void AddAmount(const std::string& key, double amount);

  // Returns the sum of the measurements with the given key.
  double GetTotal(const std::string& key) const;

 private:
  // Maps keys to aggregated measurements.
  std::unordered_map<std::string, double> measurements_;
};
