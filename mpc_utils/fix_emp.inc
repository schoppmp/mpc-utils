#ifndef MPC_UTILS_FIX_EMP_INC_
#define MPC_UTILS_FIX_EMP_INC_

// This header includes EMP headers (currently only aes.h) and ensures they
// work correctly without needing to set special COPTS. Note that if EMP is
// *used*, the resulting libraries will still require AES and SSE, but it's not
// needed at compile time with this header.

// EMP can only be included if __AES__ is defined, even if AES-NI is not needed.
#ifdef __AES__
#define MPC_UTILS_AES_WAS_DEFINED
#else
#define __AES__
#endif

#include "emp-tool/garble/aes.h"

#ifdef MPC_UTILS_AES_WAS_DEFINED
#undef MPC_UTILS_AES_WAS_DEFINED
#else
#undef __AES__
#endif

#endif  // MPC_UTILS_FIX_EMP_INC_
