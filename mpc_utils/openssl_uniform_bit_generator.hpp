// A UniformRandomBitGenerator based on OpenSSL. It uses an internal randomness
// buffer of 1MiB that gets refreshed whenever needed.
//
// See also:
// https://en.cppreference.com/w/cpp/numeric/random/UniformRandomBitGenerator

#ifndef MPC_UTILS_OPEN_SSL_UNIFORM_BIT_GENERATOR_HPP
#define MPC_UTILS_OPEN_SSL_UNIFORM_BIT_GENERATOR_HPP

#include <limits>
#include <vector>
#include "openssl/rand.h"

namespace mpc_utils {

class OpenSSLUniformBitGenerator {
 public:
  using result_type = uint64_t;

  OpenSSLUniformBitGenerator() : buffer_(kBufSize), elements_used_(kBufSize){};

  result_type operator()() {
    if (elements_used_ >= kBufSize) {
      RAND_bytes(reinterpret_cast<uint8_t *>(buffer_.data()),
                 kBufSize * sizeof(result_type));
      elements_used_ = 0;
    }
    result_type result = buffer_[elements_used_];
    elements_used_++;
    return result;
  }

  static constexpr result_type min() { return 0; }

  static constexpr result_type max() {
    return std::numeric_limits<result_type>::max();
  }

 private:
  // 1 MiB of buffered random numbers.
  static const int kBufSize = (1 << 20) / sizeof(result_type);

  std::vector<result_type> buffer_;
  uint64_t elements_used_;
};

}  // namespace mpc_utils

#endif  // MPC_UTILS_OPEN_SSL_UNIFORM_BIT_GENERATOR_HPP
