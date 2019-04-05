#include "mpc_utils/comm_channel_oblivc_adapter.hpp"
#include "gtest/gtest.h"
#include "mpc_utils/status_matchers.h"
#include "mpc_utils/testing/comm_channel_test_helper.hpp"

extern "C" {
#include "obliv.h"
// Defined in obliv.oh, but we want to avoid using oblivcc just for this test.
int ocBroadcastInt(int v, int party);
}

namespace mpc_utils {
namespace {

void DummyCircuit(void* args) {
  int* result = reinterpret_cast<int*>(args);
  *result = ocBroadcastInt(*result, 2);
}

using mpc_utils::testing::CommChannelTestHelper;

TEST(CommChannelOblivc, SendSingleInteger) {
  CommChannelTestHelper helper(false);

  boost::thread thread1([&helper] {
    int x1 = 42;
    ASSERT_OK_AND_ASSIGN(ProtocolDesc pd1, CommChannelOblivCAdapter::Connect(
                                               *helper.GetChannel(1)));
    setCurrentParty(&pd1, 2);
    execYaoProtocol(&pd1, &DummyCircuit, &x1);
    cleanupProtocol(&pd1);
  });
  boost::thread_guard<> g(thread1);

  int x0 = 23;
  ASSERT_OK_AND_ASSIGN(ProtocolDesc pd0, CommChannelOblivCAdapter::Connect(
                                             *helper.GetChannel(0)));
  setCurrentParty(&pd0, 1);
  execYaoProtocol(&pd0, &DummyCircuit, &x0);
  cleanupProtocol(&pd0);
  EXPECT_EQ(x0, 42);
  EXPECT_EQ(tcp2PBytesSent(&pd0), 0);
}

TEST(CommChannelOblivc, SendSingleIntegerMeasured) {
  CommChannelTestHelper helper(true);

  boost::thread thread1([&helper] {
    int x1 = 42;
    ASSERT_OK_AND_ASSIGN(ProtocolDesc pd1, CommChannelOblivCAdapter::Connect(
                                               *helper.GetChannel(1)));
    setCurrentParty(&pd1, 2);
    execYaoProtocol(&pd1, &DummyCircuit, &x1);
    cleanupProtocol(&pd1);
  });
  boost::thread_guard<> g(thread1);

  int x0 = 23;
  ASSERT_OK_AND_ASSIGN(ProtocolDesc pd0, CommChannelOblivCAdapter::Connect(
                                             *helper.GetChannel(0)));
  setCurrentParty(&pd0, 1);
  execYaoProtocol(&pd0, &DummyCircuit, &x0);
  cleanupProtocol(&pd0);
  EXPECT_EQ(x0, 42);
  EXPECT_GT(tcp2PBytesSent(&pd0), 0);
}

}  // namespace
}  // namespace mpc_utils
