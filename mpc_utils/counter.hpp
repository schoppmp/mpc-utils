// (C) Copyright 2008 CodeRage, LLC (turkanis at coderage dot com)
// (C) Copyright 2005-2007 Jonathan Turkanis
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

// mpc_utils: Based on Boost 1.69.0-2. Changes:
// - Remove counting of lines
// - Use 64-bit counter.

#ifndef MPC_UTILS_COUNTING_FILTER_HPP_
#define MPC_UTILS_COUNTING_FILTER_HPP_

#if defined(_MSC_VER)
#pragma once
#endif

#include "boost/iostreams/categories.hpp"
#include "boost/iostreams/char_traits.hpp"
#include "boost/iostreams/operations.hpp"
#include "boost/iostreams/pipeline.hpp"

// Must come last.
#include "boost/iostreams/detail/config/disable_warnings.hpp"  // VC7.1 C4244.

namespace mpc_utils {

//
// Template name: basic_counter.
// Template parameters:
//      Ch - The character type.
// Description: Filter which counts lines and characters.
//
template <typename Ch>
class basic_counter {
 public:
  typedef Ch char_type;
  struct category : boost::iostreams::dual_use,
                    boost::iostreams::filter_tag,
                    boost::iostreams::multichar_tag,
                    boost::iostreams::optimally_buffered_tag {};
  explicit basic_counter(int64_t first_char = 0) : chars_(first_char) {}
  int64_t characters() const { return chars_; }
  std::streamsize optimal_buffer_size() const { return 0; }

  template <typename Source>
  std::streamsize read(Source& src, char_type* s, std::streamsize n) {
    std::streamsize result = boost::iostreams::read(src, s, n);
    if (result == -1) return -1;
    chars_ += result;
    return result;
  }

  template <typename Sink>
  std::streamsize write(Sink& snk, const char_type* s, std::streamsize n) {
    std::streamsize result = boost::iostreams::write(snk, s, n);
    chars_ += result;
    return result;
  }

 private:
  int64_t chars_;
};
BOOST_IOSTREAMS_PIPABLE(basic_counter, 1)

typedef basic_counter<char> counter;
typedef basic_counter<wchar_t> wcounter;

}  // namespace mpc_utils

#include "boost/iostreams/detail/config/enable_warnings.hpp"

#endif  // #ifndef MPC_UTILS_COUNTING_FILTER_HPP_
