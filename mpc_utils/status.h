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

// Define some canonical error helpers.
inline Status UnimplementedError(google::protobuf::StringPiece message) {
  return Status(error::UNIMPLEMENTED, message);
}
inline Status InternalError(google::protobuf::StringPiece message) {
  return Status(error::INTERNAL, message);
}
inline Status FailedPreconditionError(google::protobuf::StringPiece message) {
  return Status(error::FAILED_PRECONDITION, message);
}
inline Status InvalidArgumentError(google::protobuf::StringPiece message) {
  return Status(error::INVALID_ARGUMENT, message);
}

}

#endif //MPC_UTILS_STATUS_H
