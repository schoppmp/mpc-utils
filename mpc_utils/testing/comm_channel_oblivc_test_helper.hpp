// A helper class that can be used in tests that need to connect two parties via
// a comm_channel.
#pragma once

#include <limits>
#include <random>
#include "absl/memory/memory.h"
#include "boost/thread.hpp"
#include "gtest/gtest.h"
#include "mpc_utils/comm_channel.hpp"
#include "mpc_utils/party.hpp"
extern "C" {
#include <obliv.h>
}

namespace mpc_utils {
namespace testing {

class CommChannelOblivcTest : public ::testing::Test {
 protected:
  CommChannelOblivcTest() {}
  void SetUp(bool measure_communication) {
    mpc_config conf;
    std::random_device rd;
    std::uniform_int_distribution<> dis(1025,
                                        std::numeric_limits<uint16_t>::max());
    uint16_t port = dis(rd);
    conf.servers = {server_info("127.0.0.23", port),
                    server_info("127.0.0.42", port)};
    conf.party_id = 0;
    party0 = absl::make_unique<party>(conf);
    conf.party_id = 1;
    party1 = absl::make_unique<party>(conf);
    boost::thread thread1([this, measure_communication] {
      channel1 = absl::WrapUnique(
          new comm_channel(party1->connect_to(0, measure_communication)));
      setCurrentParty(&pd1, 2);
      channel1->connect_to_oblivc(pd1);
    });
    boost::thread_guard<> g(thread1);
    channel0 = absl::WrapUnique(
        new comm_channel(party0->connect_to(1, measure_communication)));
    setCurrentParty(&pd0, 1);
    channel0->connect_to_oblivc(pd0);
  }

  std::unique_ptr<comm_channel> channel0, channel1;
  std::unique_ptr<party> party0, party1;
  ProtocolDesc pd0, pd1;
};

}  // namespace testing
}  // namespace mpc_utils