#include "comm_channel.hpp"

#include "absl/memory/memory.h"
#include "gtest/gtest.h"
#include "mpc_utils/testing/comm_channel_test_helper.hpp"

namespace mpc_utils {
namespace {

using mpc_utils::testing::CommChannelTestHelper;

TEST(CommChannel, SendSingleInteger) {
  CommChannelTestHelper helper(false);
  boost::thread thread1([&helper] {
    int x = 42;
    helper.GetChannel(1)->send(x);
    helper.GetChannel(1)->flush();
  });
  boost::thread_guard<> g(thread1);
  int y;
  helper.GetChannel(0)->recv(y);
  EXPECT_EQ(y, 42);
}

TEST(CommChannel, SendRecv) {
  CommChannelTestHelper helper(false);
  int y0, y1;
  {
    boost::thread thread1([&helper, &y1] {
      int x1 = 42;
      helper.GetChannel(1)->send_recv(x1, y1);
    });
    boost::thread_guard<> g(thread1);
    int x0 = 23;
    helper.GetChannel(0)->send_recv(x0, y0);
  }
  EXPECT_EQ(y1, 23);
  EXPECT_EQ(y0, 42);
}

TEST(CommChannel, SendSingleIntegerMeasured) {
  CommChannelTestHelper helper(true);
  boost::thread thread1([&helper] {
    int x = 42;
    helper.GetChannel(1)->send(x);
    helper.GetChannel(1)->flush();
  });
  boost::thread_guard<> g(thread1);
  int y;
  helper.GetChannel(0)->recv(y);
  EXPECT_EQ(y, 42);
  EXPECT_EQ(helper.GetChannel(0)->get_num_bytes_sent(), 0);
  EXPECT_EQ(helper.GetChannel(0)->get_num_bytes_received(), sizeof(int));
  EXPECT_EQ(helper.GetChannel(1)->get_num_bytes_sent(), sizeof(int));
  EXPECT_EQ(helper.GetChannel(1)->get_num_bytes_received(), 0);
}

TEST(CommChannel, SendRecvMeasured) {
  CommChannelTestHelper helper(true);
  int y0, y1;
  {
    boost::thread thread1([&helper, &y1] {
      int x1 = 42;
      helper.GetChannel(1)->send_recv(x1, y1);
    });
    boost::thread_guard<> g(thread1);
    int x0 = 23;
    helper.GetChannel(0)->send_recv(x0, y0);
  }
  EXPECT_EQ(y1, 23);
  EXPECT_EQ(y0, 42);
  EXPECT_EQ(helper.GetChannel(0)->get_num_bytes_sent(), sizeof(int));
  EXPECT_EQ(helper.GetChannel(0)->get_num_bytes_received(), sizeof(int));
  EXPECT_EQ(helper.GetChannel(1)->get_num_bytes_sent(), sizeof(int));
  EXPECT_EQ(helper.GetChannel(1)->get_num_bytes_received(), sizeof(int));
}

}  // namespace
}  // namespace mpc_utils