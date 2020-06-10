#include "mpc_utils/comm_channel_emp_adapter.hpp"

#include <iostream>
#include <random>

#include "boost/thread.hpp"
#include "boost/thread/thread_guard.hpp"
#include "emp-ot/test/test.h"
#include "gtest/gtest.h"
#include "mpc_utils/status_matchers.h"
#include "mpc_utils/testing/comm_channel_test_helper.hpp"

namespace mpc_utils {
namespace {

void RunParty(int party, testing::CommChannelTestHelper *helper, bool direct) {
  int length = 1 << 15;
  int ot_party = party == 0 ? ALICE : BOB;
  ASSERT_OK_AND_ASSIGN(auto io, CommChannelEMPAdapter::Create(
                                    helper->GetChannel(party), direct));
  test_ot<CommChannelEMPAdapter, OTNP>(io.get(), ot_party, 1000);
  test_ot<CommChannelEMPAdapter, SHOTExtension>(io.get(), ot_party, length);
  test_cot<CommChannelEMPAdapter, SHOTExtension>(io.get(), ot_party, length);
  test_rot<CommChannelEMPAdapter, SHOTExtension>(io.get(), ot_party, length);
}

TEST(CommChannelEMPAdapter, TestDirect) {
  testing::CommChannelTestHelper helper(false);
  boost::thread thread1([&helper] { RunParty(0, &helper, true); });
  boost::thread_guard<> g(thread1);
  RunParty(1, &helper, true);
}

TEST(CommChannelEMPAdapter, TestIndirect) {
  testing::CommChannelTestHelper helper(false);
  boost::thread thread1([&helper] { RunParty(0, &helper, false); });
  boost::thread_guard<> g(thread1);
  RunParty(1, &helper, false);
}

TEST(CommChannelEMPAdapter, TestMeasured) {
  testing::CommChannelTestHelper helper(true);
  boost::thread thread1([&helper] { RunParty(0, &helper, false); });
  boost::thread_guard<> g(thread1);
  RunParty(1, &helper, false);
  ASSERT_GT(helper.GetChannel(0)->get_num_bytes_sent(), 0);
  ASSERT_GT(helper.GetChannel(1)->get_num_bytes_sent(), 0);
  ASSERT_EQ(helper.GetChannel(0)->get_num_bytes_sent(),
            helper.GetChannel(1)->get_num_bytes_received());
  ASSERT_EQ(helper.GetChannel(1)->get_num_bytes_sent(),
            helper.GetChannel(0)->get_num_bytes_received());
}

// TODO: Add a test for EMP Garbled Circuits.

TEST(CommChannelEMPAdapter, FailsIfChannelIsNull) {
  auto status = CommChannelEMPAdapter::Create(nullptr, false);
  EXPECT_FALSE(status.ok());
  EXPECT_EQ(status.status().message(), "`channel` must not be NULL");
}

TEST(CommChannelEMPAdapter, FailsIfMeasuredAndDirect) {
  testing::CommChannelTestHelper helper(true);
  auto status = CommChannelEMPAdapter::Create(helper.GetChannel(0), true);
  EXPECT_FALSE(status.ok());
  EXPECT_EQ(status.status().message(),
            "`direct` must be false if `channel->is_measured()`");
}

}  // namespace

}  // namespace mpc_utils