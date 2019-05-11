#include "mpc_utils/testing/comm_channel_test_helper.hpp"
#include <limits>
#include <random>
#include "absl/memory/memory.h"
#include "boost/thread.hpp"
#include "mpc_utils/openssl_uniform_bit_generator.hpp"

namespace mpc_utils {
namespace testing {

CommChannelTestHelper::CommChannelTestHelper(bool measure_communication) {
  mpc_config conf;
  OpenSSLUniformBitGenerator rd;
  std::uniform_int_distribution<> dis(1025,
                                      std::numeric_limits<uint16_t>::max());
  uint16_t port;
  while (true) {
    // Try connecting to random ports until successful.
    try {
      port = dis(rd);
      conf.servers = {server_info("127.0.0.23", port),
                      server_info("127.0.0.42", port)};
      conf.party_id = 0;
      party0_ = absl::make_unique<party>(conf);
      conf.party_id = 1;
      party1_ = absl::make_unique<party>(conf);

      // Catch any exceptions thrown by thread1.
      std::exception_ptr thread_eptr = nullptr;
      boost::thread thread1([this, measure_communication, &thread_eptr] {
        try {
          channel1_ = absl::WrapUnique(new comm_channel(party1_->connect_to(
              0, measure_communication, true, /*sleep_time=*/500,
              /*num_tries=*/20)));  // Try connecting for 10s.
        } catch (boost::system::system_error& ex) {
          if (ex.code().value() == boost::system::errc::connection_refused) {
            thread_eptr = std::current_exception();
          }
        }
      });
      channel0_ = absl::WrapUnique(
          new comm_channel(party0_->connect_to(1, measure_communication)));
      thread1.join();
      if (thread_eptr) {
        continue;
      }
    } catch (boost::system::system_error& ex) {
      if (ex.code().value() == boost::system::errc::address_in_use) {
        continue;
      }
    }
    break;
  };
}

comm_channel* CommChannelTestHelper::GetChannel(int party_id) {
  if (party_id == 0) return channel0_.get();
  if (party_id == 1) return channel1_.get();
  return nullptr;
}

}  // namespace testing
}  // namespace mpc_utils