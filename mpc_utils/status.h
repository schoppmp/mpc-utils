// Convenience header to bring Status and StatusOr into the mpc_utils namespace.

#ifndef MPC_UTILS_STATUS_H
#define MPC_UTILS_STATUS_H

#include "google/protobuf/stubs/status.h"

namespace mpc_utils {

// Import Status from protobuf
using ::google::protobuf::util::Status;

namespace error {

using namespace ::google::protobuf::util::error;

}

}

#endif //MPC_UTILS_STATUS_H
