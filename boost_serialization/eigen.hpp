#pragma once

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <boost/serialization/vector.hpp>

namespace boost {
namespace serialization {
// Dense matrices
// Adapted from Stackoverflow by Shmuel Levine, Licensed CC-BY-SA 3.0
// https://stackoverflow.com/a/22903063
template <class Archive, class S, int Rows_, int Cols_, int Options, int MaxRows_,
          int MaxCols_>
inline void save(
    Archive& ar,
    const Eigen::Matrix<S, Rows_, Cols_, Options, MaxRows_, MaxCols_>& g,
    const unsigned int version) {
  bool col_major = (Options | Eigen::ColMajor) ? true : false;
  Eigen::Index rows = g.rows();
  Eigen::Index cols = g.cols();

  ar & col_major;
  ar& rows;
  ar& cols;
  ar& boost::serialization::make_array(g.data(), rows * cols);
}

template <class Archive, class S, int Rows_, int Cols_, int Options, int MaxRows_,
          int MaxCols_>
inline void load(Archive& ar,
                 Eigen::Matrix<S, Rows_, Cols_, Options, MaxRows_, MaxCols_>& g,
                 const unsigned int version) {
  bool col_major;
  Eigen::Index rows, cols;

  ar & col_major;
  ar& rows;
  ar& cols;
  if (col_major != (Options | Eigen::ColMajor)) {
    std::swap(rows, cols);
  }
  g.resize(rows, cols);
  ar& boost::serialization::make_array(g.data(), rows * cols);
  if (col_major != (Options | Eigen::ColMajor)) {
    g.transposeInPlace();
  }
}

template <class Archive, class S, int Rows_, int Cols_, int Ops_, int MaxRows_,
          int MaxCols_>
inline void serialize(
    Archive& ar, Eigen::Matrix<S, Rows_, Cols_, Ops_, MaxRows_, MaxCols_>& g,
    const unsigned int version) {
  split_free(ar, g, version);
}

// sparse matrices
template <typename Archive, typename T, typename StorageIndex>
inline void serialize(Archive& ar, Eigen::Triplet<T, StorageIndex>& triplet,
                      const unsigned int version) {
  StorageIndex i = triplet.row(), j = triplet.col();
  T val = triplet.value();
  ar& i;
  ar& j;
  ar& val;
  triplet = Eigen::Triplet<T, StorageIndex>(i, j, val);
}

template <typename Archive, typename T, int Ops_, typename StorageIndex_>
inline void save(Archive& ar,
                 const Eigen::SparseMatrix<T, Ops_, StorageIndex_>& mat,
                 const unsigned int version) {
   Eigen::Index rows = mat.rows();
   Eigen::Index cols = mat.cols();
   ar & rows;
   ar & cols;
  // TODO: maybe there's a way to save the intermediate copy?
  std::vector<Eigen::Triplet<T, StorageIndex_>> triplets;
  for (Eigen::Index k = 0; k < mat.outerSize(); k++) {
    for (typename Eigen::SparseMatrix<T, Ops_, StorageIndex_>::InnerIterator it(
             mat, k);
         it; ++it) {
      triplets.push_back(
          Eigen::Triplet<T, StorageIndex_>(it.row(), it.col(), it.value()));
    }
  }
  ar& triplets;
}

template <typename Archive, typename T, int Ops_, typename StorageIndex>
inline void load(Archive& ar, Eigen::SparseMatrix<T, Ops_, StorageIndex>& mat,
                 const unsigned int version) {
  std::vector<Eigen::Triplet<T, StorageIndex>> triplets;
  Eigen::Index rows, cols;
  ar& rows;
  ar & cols;
  ar& triplets;
  mat.resize(rows, cols);
  mat.setFromTriplets(triplets.begin(), triplets.end());
}

template <typename Archive, typename T, int Ops_, typename StorageIndex_>
inline void serialize(Archive& ar,
                      Eigen::SparseMatrix<T, Ops_, StorageIndex_>& mat,
                      const unsigned int version) {
  split_free(ar, mat, version);
}

}  // namespace serialization
}  // namespace boost
