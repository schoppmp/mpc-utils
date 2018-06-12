#pragma once

#include <boost/serialization/vector.hpp>
#include <NTL/ZZ.h>

namespace boost { namespace serialization {
// NTL::ZZ
template<class Archive>
inline void save(
	Archive & ar,
	const NTL::ZZ & z,
	const unsigned int /* file_version */
){
  std::vector<unsigned char> buf(NTL::NumBytes(z));
  NTL::BytesFromZZ(buf.data(), z, buf.size());
  ar & buf;
}
template<class Archive>
inline void load(
	Archive & ar,
	NTL::ZZ & z,
	const unsigned int /* file_version */
){
  std::vector<unsigned char> buf;
  ar & buf;
  NTL::ZZFromBytes(z, buf.data(), buf.size());
}

// NTL::ZZ_p
template<class Archive>
inline void save(
	Archive & ar,
	const NTL::ZZ_p & zp,
	const unsigned int /* file_version */
){
  ar & NTL::conv<NTL::ZZ>(zp);
}
template<class Archive>
inline void load(
	Archive & ar,
	NTL::ZZ_p & zp,
	const unsigned int /* file_version */
){
  NTL::ZZ z;
  ar & z;
  NTL::conv(zp, z);
}

// NTL::ZZ_pX
template<class Archive>
inline void serialize(
	Archive & ar,
	NTL::ZZ_pX & p,
	const unsigned int /* file_version */
){
  long n = NTL::deg(p);
  ar & n;
  p.SetLength(n + 1);
  if(n >= 0) {
    for(long i = 0; i <= n; i++) {
      ar & p[i];
    }
  }
}

// NTL::Vec
template<class Archive, class T>
inline void serialize(
	Archive & ar,
	NTL::Vec<T> & v,
	const unsigned int /* file_version */
){
  long n = v.length();
  ar & n;
  v.SetLength(n);
  for(auto& el : v) {
    ar & el;
  }
}

}} // namespace boost::serialization

// register optional splits
BOOST_SERIALIZATION_SPLIT_FREE(NTL::ZZ)
BOOST_SERIALIZATION_SPLIT_FREE(NTL::ZZ_p)
