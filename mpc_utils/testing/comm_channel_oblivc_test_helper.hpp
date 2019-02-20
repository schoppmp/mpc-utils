// A helper class that can be used in tests that need to connect two parties via
// a comm_channel.
#pragma once

#include "mpc_utils/comm_channel.hpp"
#include "mpc_utils/party.hpp"
extern "C" {
#include <obliv.h>
}

namespace mpc_utils {
namespace testing {

class CommChannelOblivcTestHelper {
 public:
  CommChannelOblivcTestHelper(bool measure_communication);

  comm_channel* GetChannel(int party_id) {
    if (party_id == 0) return channel0_.get();
    if (party_id == 1) return channel1_.get();
    return nullptr;
  }

  ProtocolDesc* GetProtocolDesc(int party_id) {
    if (party_id == 0) return &pd0_;
    if (party_id == 1) return &pd1_;
    return nullptr;
  }

 private:
  std::unique_ptr<comm_channel> channel0_, channel1_;
  std::unique_ptr<party> party0_, party1_;
  ProtocolDesc pd0_, pd1_;
};

}  // namespace testing
}  // namespace mpc_utils