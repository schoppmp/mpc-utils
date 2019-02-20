#include "gtest/gtest.h"
#include "mpc_utils/testing/comm_channel_oblivc_test_helper.hpp"

extern "C" {
#include <obliv.h>
// Defined in obliv.oh, but we want to avoid using oblivcc just for this test.
int ocBroadcastInt(int v, int party);
}

namespace mpc_utils {
namespace {

void DummyCircuit(void *args) {
  int* result = reinterpret_cast<int*>(args);
  *result = ocBroadcastInt(*result, 2);
}

using mpc_utils::testing::CommChannelOblivcTestHelper;

TEST(CommChannelOblivc, SendSingleInteger) {
  CommChannelOblivcTestHelper helper(false);
  ProtocolDesc *pd0 = helper.GetProtocolDesc(0);
  ProtocolDesc *pd1 = helper.GetProtocolDesc(1);

  boost::thread thread1([this, pd1] {
    int x1 = 42;
    execYaoProtocol(pd1, &DummyCircuit, &x1);
    cleanupProtocol(pd1);
  });
  boost::thread_guard<> g(thread1);

  int x0 = 23;
  execYaoProtocol(pd0, &DummyCircuit, &x0);
  cleanupProtocol(pd0);
  EXPECT_EQ(x0, 42);
  EXPECT_EQ(tcp2PBytesSent(pd0), 0);
}

TEST(CommChannelOblivc, SendSingleIntegerMeasured) {
  CommChannelOblivcTestHelper helper(true);
  ProtocolDesc *pd0 = helper.GetProtocolDesc(0);
  ProtocolDesc *pd1 = helper.GetProtocolDesc(1);

  boost::thread thread1([this, pd1] {
    int x1 = 42;
    execYaoProtocol(pd1, &DummyCircuit, &x1);
    cleanupProtocol(pd1);
  });
  boost::thread_guard<> g(thread1);

  int x0 = 23;
  execYaoProtocol(pd0, &DummyCircuit, &x0);
  cleanupProtocol(pd0);
  EXPECT_EQ(x0, 42);
  EXPECT_GT(tcp2PBytesSent(pd0), 0);
}

}
}
