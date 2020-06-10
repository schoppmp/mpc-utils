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
  bool keep_trying = true;
  while (keep_trying) {
    // Try connecting to random ports until successful.
    port = dis(rd);
    conf.servers = {server_info("127.0.0.23", port),
                    server_info("127.0.0.42", port)};
    conf.party_id = 0;
    party0_ = absl::make_unique<party>(conf);
    conf.party_id = 1;
    party1_ = absl::make_unique<party>(conf);

    // Catch any exceptions thrown by thread1.
    boost::exception_ptr thread_eptr;
    boost::thread thread1([this, measure_communication, &thread_eptr] {
      try {
        channel0_ = absl::WrapUnique(
            new comm_channel(party0_->connect_to(1, measure_communication)));
      } catch (boost::system::system_error& ex) {
        if (ex.code().value() == boost::system::errc::address_in_use) {
          // Set thread_eptr to notify client that we can't accept.
          thread_eptr = boost::current_exception();
        }
      }
    });
    while (keep_trying) {
      try {
        channel1_ = absl::WrapUnique(new comm_channel(party1_->connect_to(
            0, measure_communication, true, /*sleep_time=*/500,
            /*num_tries=*/10)));  // Try connecting for 5s.
        keep_trying = false;      // Connection successful!
        thread1.join();
      } catch (boost::system::system_error& ex) {
        if (ex.code().value() == boost::system::errc::connection_refused) {
          if (thread_eptr) {
            // Server could not accept -> Retry with both parties and different
            // port.
            thread1.join();
            break;
          } else {
            // Couldn't connect, but server does not seem to have a problem ->
            // Only client retries.
            continue;
          }
        }
      }
    }
    thread1.join();
    if (thread_eptr) {
      continue;
    }
    break;
  }
}

comm_channel* CommChannelTestHelper::GetChannel(int party_id) {
  if (party_id == 0) return channel0_.get();
  if (party_id == 1) return channel1_.get();
  return nullptr;
}

}  // namespace testing
}  // namespace mpc_utils