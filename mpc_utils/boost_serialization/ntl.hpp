#ifndef MPC_UTILS_BOOST_SERIALIZATION_NTL_HPP_
#define MPC_UTILS_BOOST_SERIALIZATION_NTL_HPP_

#include "NTL/ZZ.h"
#include "NTL/ZZ_p.h"
#include "NTL/ZZ_pX.h"
#include "NTL/lzz_p.h"
#include "boost/serialization/split_free.hpp"
#include "boost/serialization/vector.hpp"

namespace boost {
namespace serialization {
// NTL::ZZ
template <class Archive>
inline void save(Archive& ar, const NTL::ZZ& z,
                 const unsigned int /* file_version */
) {
  std::vector<unsigned char> buf(NTL::NumBytes(z));
  NTL::BytesFromZZ(buf.data(), z, static_cast<long>(buf.size()));
  ar& buf;
}
template <class Archive>
inline void load(Archive& ar, NTL::ZZ& z, const unsigned int /* file_version */
) {
  std::vector<unsigned char> buf;
  ar& buf;
  NTL::ZZFromBytes(z, buf.data(), static_cast<long>(buf.size()));
}

// NTL::ZZ_p, NTL::zz_p
template <typename T, typename Archive, typename R = typename T::rep_type>
inline void save(Archive& ar, const T& zp, const unsigned int /* file_version */
) {
  R rep;
  NTL::conv(rep, zp);
  ar& rep;
}
template <typename T, typename Archive, typename R = typename T::rep_type>
inline void load(Archive& ar, T& zp, const unsigned int /* file_version */
) {
  R rep;
  ar& rep;
  NTL::conv(zp, rep);
}

// NTL::ZZ_pX
template <class Archive>
inline void serialize(Archive& ar, NTL::ZZ_pX& p,
                      const unsigned int /* file_version */
) {
  long n = NTL::deg(p);
  ar& n;
  p.SetLength(n + 1);
  if (n >= 0) {
    for (long i = 0; i <= n; i++) {
      ar& p[i];
    }
  }
}

// NTL::Vec
template <class Archive, class T>
inline void serialize(Archive& ar, NTL::Vec<T>& v,
                      const unsigned int /* file_version */
) {
  long n = v.length();
  ar& n;
  v.SetLength(n);
  for (auto& el : v) {
    ar& el;
  }
}

}  // namespace serialization
}  // namespace boost

// register optional splits
BOOST_SERIALIZATION_SPLIT_FREE(NTL::ZZ)
BOOST_SERIALIZATION_SPLIT_FREE(NTL::ZZ_p)
BOOST_SERIALIZATION_SPLIT_FREE(NTL::zz_p)

#endif  // MPC_UTILS_BOOST_SERIALIZATION_NTL_HPP_