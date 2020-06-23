#include "mpc_utils/comm_channel_emp_adapter.hpp"

#include "absl/memory/memory.h"
#include "emp-tool/io/net_io_channel.h"
#include "mpc_utils/canonical_errors.h"

namespace mpc_utils {

StatusOr<std::unique_ptr<CommChannelEMPAdapter>>
CommChannelEMPAdapter::Create(comm_channel *channel, bool direct) {
  if (channel == nullptr) {
    return InvalidArgumentError("`channel` must not be NULL");
  }
  if (direct && channel->is_measured()) {
    return InvalidArgumentError(
        "`direct` must be false if `channel->is_measured()`");
  }

  // EMP crashes if we don't call this.
  emp::initialize_relic();

  std::unique_ptr<emp::NetIO> net_io;
  if (direct) {
    // Connect directly using EMP for performance.
    // Party with lower ID accepts, higher ID connects.
    if (channel->get_id() < channel->get_peer_id()) {
      net_io = absl::make_unique<emp::NetIO>(
          nullptr, channel->get_local_info().port, /*quiet=*/true);
    } else {
      server_info info = channel->get_peer_info();
      net_io = absl::make_unique<emp::NetIO>(info.host.c_str(), info.port,
                                             /*quiet=*/true);
    }
  }
  return absl::WrapUnique(
      new CommChannelEMPAdapter(channel, std::move(net_io)));
};

CommChannelEMPAdapter::CommChannelEMPAdapter(comm_channel *channel,
                                             std::unique_ptr<emp::NetIO> net_io)
    : channel_(channel), net_io_(std::move(net_io)){};

} // namespace mpc_utils
