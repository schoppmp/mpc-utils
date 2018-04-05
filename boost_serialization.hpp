#include <boost/serialization/vector.hpp>


#ifdef MPC_UTILS_USE_NTL
#include <NTL/ZZ.h>
#endif

namespace boost { namespace serialization {

#ifdef MPC_UTILS_USE_NTL
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
#endif // MPC_UTILS_USE_NTL

}}

BOOST_SERIALIZATION_SPLIT_FREE(NTL::ZZ)
