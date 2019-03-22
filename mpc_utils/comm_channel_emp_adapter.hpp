// Addapter for the comm_channel class to use with functions in EMP that require
// an IOChannel.

#ifndef MPC_UTILS_COMM_CHANNEL_EMP_H
#define MPC_UTILS_COMM_CHANNEL_EMP_H

#include "emp-tool/io/io_channel.h"
#include "mpc_utils/comm_channel.hpp"

namespace mpc_utils {

class CommChannelEMPAdapter : public emp::IOChannel<CommChannelEMPAdapter> {
 public:
  // Constructs an adapter for a given comm_channel. The adapted channel is not
  // owned and it's the caller's responsibility to ensure it outlives the
  // adatper.
  CommChannelEMPAdapter(comm_channel *channel) : channel_(channel){};

  inline void send_data(const void *data, int len) {
    channel_->write(reinterpret_cast<const char *>(data), len);
  }

  inline void recv_data(void *data, int len) {
    channel_->read(reinterpret_cast<char *>(data), len);
  }

  inline void sync() { channel_->sync(); }

  inline void flush() { channel_->flush(); }

 private:
  comm_channel *channel_;
};

}  // namespace mpc_utils

#endif  // MPC_UTILS_COMM_CHANNEL_EMP_H
