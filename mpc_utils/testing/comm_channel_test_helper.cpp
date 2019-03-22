#include "comm_channel_test_helper.hpp"
#include <limits>
#include <random>
#include "absl/memory/memory.h"
#include "boost/thread.hpp"

namespace mpc_utils {
namespace testing {

CommChannelTestHelper::CommChannelTestHelper(bool measure_communication) {
  mpc_config conf;
  std::random_device rd;
  std::uniform_int_distribution<> dis(1025,
                                      std::numeric_limits<uint16_t>::max());
  uint16_t port = dis(rd);
  conf.servers = {server_info("127.0.0.23", port),
                  server_info("127.0.0.42", port)};
  conf.party_id = 0;
  party0_ = absl::make_unique<party>(conf);
  conf.party_id = 1;
  party1_ = absl::make_unique<party>(conf);
  boost::thread thread1([this, measure_communication] {
    channel1_ = absl::WrapUnique(
        new comm_channel(party1_->connect_to(0, measure_communication)));
  });
  boost::thread_guard<> g(thread1);
  channel0_ = absl::WrapUnique(
      new comm_channel(party0_->connect_to(1, measure_communication)));
}

comm_channel* CommChannelTestHelper::GetChannel(int party_id) {
  if (party_id == 0) return channel0_.get();
  if (party_id == 1) return channel1_.get();
  return nullptr;
}

}  // namespace testing
}  // namespace mpc_utils