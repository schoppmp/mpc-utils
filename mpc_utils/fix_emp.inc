#ifndef MPC_UTILS_FIX_EMP_INC_
#define MPC_UTILS_FIX_EMP_INC_

// This header includes EMP headers (currently only aes.h) and ensures they
// work correctly without needing to set special COPTS. Note that if EMP is
// *used*, the resulting libraries will still require AES and SSE, but it's not
// needed at compile time with this header.

// EMP headers use AES and SSE intrinsics, so we have to enable those here.
// Also ignores some warnings emitted by EMP.
#ifdef __GNUC__
#pragma GCC push_options
#pragma GCC target("aes", "sse2")
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif

// EMP can only be included if __AES__ is defined, even if AES-NI is not needed.
#ifdef __AES__
#define MPC_UTILS_AES_WAS_DEFINED
#else
#define __AES__
#endif

#include "emp-tool/garble/aes.h"

// Restore everything as it was before.
#ifdef MPC_UTILS_AES_WAS_DEFINED
#undef MPC_UTILS_AES_WAS_DEFINED
#else
#undef __AES__
#endif

#ifdef __GNUC__
#pragma GCC pop_options
#pragma GCC diagnostic pop
#endif

#endif  // MPC_UTILS_FIX_EMP_INC_
