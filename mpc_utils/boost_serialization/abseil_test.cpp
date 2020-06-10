#include "mpc_utils/boost_serialization/abseil.hpp"

#include <sstream>

#include "boost/archive/binary_iarchive.hpp"
#include "boost/archive/binary_oarchive.hpp"
#include "gtest/gtest.h"

class AbseilTest : public ::testing::Test {
 protected:
  AbseilTest() : oarchive(stream), iarchive(stream) {}

  std::stringstream stream;
  boost::archive::binary_oarchive oarchive;
  boost::archive::binary_iarchive iarchive;
};

TEST_F(AbseilTest, SerializeValue) {
  absl::uint128 x = absl::MakeUint128(23, 42);
  oarchive& x;
  absl::uint128 y = 0;
  iarchive& y;
  EXPECT_EQ(x, y);
}