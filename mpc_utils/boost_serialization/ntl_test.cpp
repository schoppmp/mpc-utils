#include "ntl.hpp"
#include <NTL/ZZ.h>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <sstream>
#include "gtest/gtest.h"

class NTLTest : public ::testing::Test {
 protected:
  NTLTest() : oarchive(stream), iarchive(stream) {}

  std::stringstream stream;
  boost::archive::binary_oarchive oarchive;
  boost::archive::binary_iarchive iarchive;
};

TEST_F(NTLTest, SerializeZZ) {
  NTL::ZZ x(5);
  oarchive& x;
  NTL::ZZ y;
  iarchive& y;
  EXPECT_EQ(x, y);
}

TEST_F(NTLTest, SerializeZZ_p) {
  NTL::ZZ_p::init(NTL::ZZ(123));
  NTL::ZZ_p x(5);
  oarchive& x;
  NTL::ZZ_p y;
  iarchive& y;
  EXPECT_EQ(x, y);
}

TEST_F(NTLTest, SerializeZZ_pX) {
  NTL::ZZ_p::init(NTL::ZZ(123));
  NTL::ZZ_pX p1;
  NTL::SetCoeff(p1, 2, 5);  // p1(x) = 5x².
  oarchive& p1;
  NTL::ZZ_pX p2;
  iarchive& p2;
  EXPECT_EQ(p1, p2);
}

TEST_F(NTLTest, SerializeVec) {
  NTL::Vec<NTL::ZZ> x;
  x.SetLength(5, NTL::ZZ(3));  // x = {3, 3, 3, 3, 3}.
  oarchive& x;
  NTL::Vec<NTL::ZZ> y;
  iarchive& y;
  EXPECT_EQ(x, y);
}
