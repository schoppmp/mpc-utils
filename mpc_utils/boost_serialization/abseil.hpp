#ifndef MPC_UTILS_BOOST_SERIALIZATION_ABSEIL_HPP_
#define MPC_UTILS_BOOST_SERIALIZATION_ABSEIL_HPP_

#include <cstdint>

#include "absl/numeric/int128.h"
#include "boost/serialization/split_free.hpp"

namespace boost {
namespace serialization {

// absl::uint128
template <class Archive>
inline void save(Archive &ar, const absl::uint128 &x,
                 const unsigned int /* file_version */
) {
  uint64_t hi, lo;
  hi = absl::Uint128High64(x);
  lo = absl::Uint128Low64(x);
  ar &hi;
  ar &lo;
}
template <class Archive>
inline void load(Archive &ar, absl::uint128 &x,
                 const unsigned int /* file_version */
) {
  uint64_t hi, lo;
  ar &hi;
  ar &lo;
  x = absl::MakeUint128(hi, lo);
}

}  // namespace serialization
}  // namespace boost

// register splits
BOOST_SERIALIZATION_SPLIT_FREE(absl::uint128)

#endif  // MPC_UTILS_BOOST_SERIALIZATION_ABSEIL_HPP_
