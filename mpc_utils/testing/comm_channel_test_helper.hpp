// A helper class that can be used in tests that need to connect two parties via
// a comm_channel.
#pragma once

#include "mpc_utils/comm_channel.hpp"
#include "mpc_utils/party.hpp"

namespace mpc_utils {
namespace testing {

class CommChannelTestHelper {
 public:
  CommChannelTestHelper(bool measure_communication);

  comm_channel* GetChannel(int party_id);

 private:
  std::unique_ptr<comm_channel> channel0_, channel1_;
  std::unique_ptr<party> party0_, party1_;
};

}  // namespace testing
}  // namespace mpc_utils