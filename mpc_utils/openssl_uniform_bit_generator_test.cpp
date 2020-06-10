#include "mpc_utils/openssl_uniform_bit_generator.hpp"

#include <random>

#include "gtest/gtest.h"

namespace mpc_utils {
namespace {

TEST(OpenSSLUniformBitGenerator, NotObviouslyBroken) {
  OpenSSLUniformBitGenerator rng;
  uint64_t a = rng(), b = rng();
  EXPECT_NE(a, b);
  EXPECT_NE(a, 0);
  EXPECT_NE(b, 0);
}

TEST(OpenSSLUniformBitGenerator, AllBitsGetSampled) {
  int num_samples = 80;  // Enough to be reasonably confident every bit gets set
                         // to 1 at least once.
  OpenSSLUniformBitGenerator rng;
  uint64_t rand = 0;
  for (int i = 0; i < num_samples; i++) {
    rand |= rng();
  }
  EXPECT_EQ(rand, 0xFFFFFFFFFFFFFFFF);
}

TEST(OpenSSLUniformBitGenerator, UniformReals) {
  int num_samples = 1 << 24;
  double rand = 0.;
  OpenSSLUniformBitGenerator rng;
  std::uniform_real_distribution<double> dist;
  for (int i = 0; i < num_samples; i++) {
    rand += dist(rng);
  }
  EXPECT_LE(std::abs(rand / num_samples - 0.5), 0.01);
}

}  // namespace
}  // namespace mpc_utils