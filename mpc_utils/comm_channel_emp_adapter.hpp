// Addapter for the comm_channel class to use with functions in EMP that require
// an IOChannel.

#ifndef MPC_UTILS_COMM_CHANNEL_EMP_ADAPTER_HPP_
#define MPC_UTILS_COMM_CHANNEL_EMP_ADAPTER_HPP_

#include "mpc_utils/fix_emp.ipp"

#include "emp-tool/io/io_channel.h"
#include "emp-tool/io/net_io_channel.h"
#include "mpc_utils/comm_channel.hpp"
#include "mpc_utils/statusor.h"

namespace mpc_utils {

class CommChannelEMPAdapter : public emp::IOChannel<CommChannelEMPAdapter> {
 public:
  // Constructs an adapter for a given comm_channel. The adapted channel is not
  // owned and it's the caller's responsibility to ensure it outlives the
  // adatper. `direct` indicates whether to use a direct EMP connection. If set
  // to true, establishes a new connection to the remote endpoint of the passed
  // comm_channel. Otherwise, all communication is routed through the passed
  // channel. Beware that EMP does not do error handling, so setting `direct` to
  // true may crash the program. For measured channels, `direct` must be false.
  static StatusOr<std::unique_ptr<CommChannelEMPAdapter>> Create(
      comm_channel *channel, bool direct);

  inline void send_data(const void *data, int len) {
    if (net_io_) {
      net_io_->send_data(data, len);
    } else {
      channel_->write(reinterpret_cast<const char *>(data), len);
    }
  }

  inline void recv_data(void *data, int len) {
    if (net_io_) {
      net_io_->recv_data(data, len);
    } else {
      channel_->read(reinterpret_cast<char *>(data), len);
    }
  }

  inline void sync() {
    if (net_io_) {
      net_io_->sync();
    } else {
      channel_->sync();
    }
  }

  inline void flush() {
    if (net_io_) {
      net_io_->flush();
    } else {
      channel_->flush();
    }
  }

 private:
  // Private constructor. Called by Create.
  CommChannelEMPAdapter(comm_channel *channel,
                        std::unique_ptr<emp::NetIO> net_io);

  // The wrapped comm_channel.
  comm_channel *channel_;

  // Direct connection using EMP if we don't need to measure communication.
  std::unique_ptr<emp::NetIO> net_io_;
};

}  // namespace mpc_utils

#endif  // MPC_UTILS_COMM_CHANNEL_EMP_ADAPTER_HPP_
