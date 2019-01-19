#include "eigen.hpp"
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <sstream>
#include "gtest/gtest.h"

class EigenTest : public ::testing::Test {
 protected:
  EigenTest() : oarchive(stream), iarchive(stream) {}

  std::stringstream stream;
  boost::archive::binary_oarchive oarchive;
  boost::archive::binary_iarchive iarchive;
};

TEST_F(EigenTest, SerializeDense) {
  Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> matrix(2, 3);
  matrix << 1, 2, 3, 4, 5, 6;
  oarchive& matrix;
  Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> matrix2;
  iarchive& matrix2;
  EXPECT_EQ(matrix, matrix2);
}

TEST_F(EigenTest, SerializeDenseRowsCols) {
  Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> matrix(2,
                                                                             3);
  matrix << 1, 2, 3, 4, 5, 6;
  oarchive& matrix;
  Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> matrix2;
  iarchive& matrix2;
  EXPECT_EQ(matrix, matrix2);
}

TEST_F(EigenTest, SerializeDenseColsRows) {
  Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> matrix(2,
                                                                             3);
  matrix << 1, 2, 3, 4, 5, 6;
  oarchive& matrix;
  Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> matrix2;
  iarchive& matrix2;
  EXPECT_EQ(matrix, matrix2);
}

class EigenTestSparse : public EigenTest {
 protected:
  EigenTestSparse()
      : triplets({
            {1, 2, 3},
            {4, 5, 6},
            {20, 30, 123},
        }) {}

  std::vector<Eigen::Triplet<int>> triplets;
};

TEST_F(EigenTestSparse, Serialize) {
  Eigen::SparseMatrix<int> matrix(200, 300);
  matrix.setFromTriplets(triplets.begin(), triplets.end());
  oarchive& matrix;
  Eigen::SparseMatrix<int> matrix2;
  iarchive& matrix2;
  for (auto& triplet : triplets) {
    EXPECT_EQ(matrix2.coeff(triplet.row(), triplet.col()), triplet.value());
  }
}

TEST_F(EigenTestSparse, SerializeRowsCols) {
  Eigen::SparseMatrix<int, Eigen::RowMajor> matrix(200, 300);
  matrix.setFromTriplets(triplets.begin(), triplets.end());
  oarchive& matrix;
  Eigen::SparseMatrix<int, Eigen::ColMajor> matrix2;
  iarchive& matrix2;
  for (auto& triplet : triplets) {
    EXPECT_EQ(matrix2.coeff(triplet.row(), triplet.col()), triplet.value());
  }
}

TEST_F(EigenTestSparse, SerializeColsRows) {
  Eigen::SparseMatrix<int, Eigen::ColMajor> matrix(200, 300);
  matrix.setFromTriplets(triplets.begin(), triplets.end());
  oarchive& matrix;
  Eigen::SparseMatrix<int, Eigen::RowMajor> matrix2;
  iarchive& matrix2;
  for (auto& triplet : triplets) {
    EXPECT_EQ(matrix2.coeff(triplet.row(), triplet.col()), triplet.value());
  }
}
