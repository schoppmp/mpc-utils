#include "mpc_utils/comm_channel_oblivc_adapter.hpp"
#include <cerrno>
#include <cstring>
#include "absl/strings/str_cat.h"
#include "mpc_utils/canonical_errors.h"
#include "mpc_utils/party.hpp"

namespace mpc_utils {

StatusOr<ProtocolDesc> CommChannelOblivCAdapter::Connect(
    const comm_channel& channel, int sleep_time, int num_tries) {
  ProtocolDesc pd;
  int peer_id = channel.get_peer_id();
  int id = channel.get_id();
  bool measure_communication = channel.is_measured();
  if (id < peer_id) {
    // Lower ID accepts.
    const auto& endpoint = channel.tcp_stream->socket().local_endpoint();
    std::string port(std::to_string(endpoint.port()));
    int result;
    if (measure_communication) {
      result = protocolAcceptTcp2PProfiled(&pd, port.c_str());
    } else {
      result = protocolAcceptTcp2P(&pd, port.c_str());
    }
    if (result == 0) {
      return pd;
    } else {
      return UnavailableError(
          absl::StrCat("Accepting Obliv-C connection on port ", port,
                       " failed: ", strerror(errno)));
    }
  } else {
    // Higher ID connects.
    int result = -1;
    const auto& endpoint = channel.tcp_stream->socket().remote_endpoint();
    std::string address = endpoint.address().to_string();
    std::string port(std::to_string(endpoint.port()));

    // Try connecting until successful or we tried `num_tries` times.
    for (int i = 0; i < num_tries || num_tries == -1; i++) {
      if (measure_communication) {
        result =
            protocolConnectTcp2PProfiled(&pd, address.c_str(), port.c_str());
      } else {
        result = protocolConnectTcp2P(&pd, address.c_str(), port.c_str());
      }
      if (result != -1) {
        break;  // Connection successful or failed permanently.
      }
      boost::this_thread::sleep_for(boost::chrono::milliseconds(sleep_time));
    }
    if (result == 0) {
      return pd;
    } else {
      return UnavailableError(absl::StrCat("Obliv-C Connection to ", address,
                                           ":", port,
                                           " failed: ", strerror(errno)));
    }
  }
}

}  // namespace mpc_utils