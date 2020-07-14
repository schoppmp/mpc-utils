// Adapter for the comm_channel class to use with Obliv-C. Currently, it just
// contains a static function that returns a ProtocolDesc, but this might change
// in the future.

#ifndef MPC_UTILS_COMM_CHANNEL_OBLIVC_ADAPTER_HPP_
#define MPC_UTILS_COMM_CHANNEL_OBLIVC_ADAPTER_HPP_

#include "mpc_utils/comm_channel.hpp"
#include "mpc_utils/statusor.h"

#ifdef MPC_ENABLE_OBLVC_ADAPTER


extern "C" {
#include "obliv.h"
};

namespace mpc_utils {

class CommChannelOblivCAdapter {
 public:
  // Connects the two parties of the passed comm_channel using Obliv-C and
  // returns the resulting ProtocolDesc. Returns StatusCode::kUnavailable if the
  // connection fails.
  static StatusOr<ProtocolDesc> Connect(
      const comm_channel& channel, int sleep_time = party::DEFAULT_SLEEP_TIME,
      int num_tries = party::DEFAULT_NUM_TRIES);
};

}  // namespace mpc_utils
#endif

#endif  // MPC_UTILS_COMM_CHANNEL_OBLIVC_ADAPTER_HPP_