#pragma once

#include <boost/serialization/vector.hpp>
#include <Eigen/Dense>

namespace boost { namespace serialization {

// From Stackoverflow by Shmuel Levine, Licensed CC-BY-SA 3.0
// https://stackoverflow.com/a/22903063
template<   class Archive,
            class S,
            int Rows_,
            int Cols_,
            int Ops_,
            int MaxRows_,
            int MaxCols_>
inline void save(
    Archive & ar,
    const Eigen::Matrix<S, Rows_, Cols_, Ops_, MaxRows_, MaxCols_> & g,
    const unsigned int version)
    {
        int rows = g.rows();
        int cols = g.cols();

        ar & rows;
        ar & cols;
        ar & boost::serialization::make_array(g.data(), rows * cols);
    }

template<   class Archive,
            class S,
            int Rows_,
            int Cols_,
            int Ops_,
            int MaxRows_,
            int MaxCols_>
inline void load(
    Archive & ar,
    Eigen::Matrix<S, Rows_, Cols_, Ops_, MaxRows_, MaxCols_> & g,
    const unsigned int version)
{
    int rows, cols;
    ar & rows;
    ar & cols;
    g.resize(rows, cols);
    ar & boost::serialization::make_array(g.data(), rows * cols);
}

template<   class Archive,
            class S,
            int Rows_,
            int Cols_,
            int Ops_,
            int MaxRows_,
            int MaxCols_>
inline void serialize(
    Archive & ar,
    Eigen::Matrix<S, Rows_, Cols_, Ops_, MaxRows_, MaxCols_> & g,
    const unsigned int version)
{
    split_free(ar, g, version);
}

}} // namespace boost::serialization
