#include "absl/memory/memory.h"
#include "comm_channel.hpp"
#include "gtest/gtest.h"

extern "C" {
#include <obliv.h>
// Defined in obliv.oh, but we want to avoid using oblivcc just for this test.
int ocBroadcastInt(int v, int party);
}

class CommChannelOblivcTest : public ::testing::Test {
 protected:
  CommChannelOblivcTest() {}
  void SetUp(bool measure_communication) {
    mpc_config conf;
    conf.servers = {server_info("127.0.0.23", 2341),
                    server_info("127.0.0.42", 2341)};
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

  static void dummy_circuit(void* args) {
    int* result = reinterpret_cast<int*>(args);
    *result = ocBroadcastInt(*result, 2);
  }

  std::unique_ptr<comm_channel> channel0, channel1;
  std::unique_ptr<party> party0, party1;
  ProtocolDesc pd0, pd1;
};

TEST_F(CommChannelOblivcTest, SendSingleInteger) {
  SetUp(false);
  boost::thread thread1([this] {
    int x1 = 42;
    execYaoProtocol(&pd1, &dummy_circuit, &x1);
    cleanupProtocol(&pd1);
  });
  boost::thread_guard<> g(thread1);
  int x0 = 23;
  execYaoProtocol(&pd0, &dummy_circuit, &x0);
  cleanupProtocol(&pd0);
  EXPECT_EQ(x0, 42);
  EXPECT_EQ(tcp2PBytesSent(&pd0), 0);
}

TEST_F(CommChannelOblivcTest, SendSingleIntegerMeasured) {
  SetUp(true);
  boost::thread thread1([this] {
    int x1 = 42;
    execYaoProtocol(&pd1, &dummy_circuit, &x1);
    cleanupProtocol(&pd1);
  });
  boost::thread_guard<> g(thread1);
  int x0 = 23;
  execYaoProtocol(&pd0, &dummy_circuit, &x0);
  cleanupProtocol(&pd0);
  EXPECT_EQ(x0, 42);
  EXPECT_GT(tcp2PBytesSent(&pd0), 0);
}
