#include "comm_channel.hpp"
#include "absl/memory/memory.h"
#include "gtest/gtest.h"

class CommChannelTest : public ::testing::Test {
 protected:
  CommChannelTest() {}
  void SetUp(bool measure_communication) {
    mpc_config conf;
    conf.servers = {server_info("127.0.0.23", 2342),
                    server_info("127.0.0.42", 2342)};
    conf.party_id = 0;
    party0 = absl::make_unique<party>(conf);
    conf.party_id = 1;
    party1 = absl::make_unique<party>(conf);
    boost::thread thread1([this, measure_communication] {
      this->channel1 = absl::WrapUnique(
          new comm_channel(this->party1->connect_to(0, measure_communication)));
    });
    boost::thread_guard<> g(thread1);
    channel0 = absl::WrapUnique(
        new comm_channel(party0->connect_to(1, measure_communication)));
  }

  std::unique_ptr<comm_channel> channel0, channel1;
  std::unique_ptr<party> party0, party1;
};

TEST_F(CommChannelTest, SendSingleInteger) {
  SetUp(false);
  boost::thread thread1([this] {
    int x = 42;
    this->channel1->send(x);
    this->channel1->flush();
  });
  boost::thread_guard<> g(thread1);
  int y;
  channel0->recv(y);
  EXPECT_EQ(y, 42);
}

TEST_F(CommChannelTest, SendRecv) {
  SetUp(false);
  int y0, y1;
  {
    boost::thread thread1([this, &y1] {
      int x1 = 42;
      this->channel1->send_recv(x1, y1);
    });
    boost::thread_guard<> g(thread1);
    int x0 = 23;
    channel0->send_recv(x0, y0);
  }
  EXPECT_EQ(y1, 23);
  EXPECT_EQ(y0, 42);
}

TEST_F(CommChannelTest, SendSingleIntegerMeasured) {
  SetUp(true);
  boost::thread thread1([this] {
    int x = 42;
    this->channel1->send(x);
    this->channel1->flush();
  });
  boost::thread_guard<> g(thread1);
  int y;
  channel0->recv(y);
  EXPECT_EQ(y, 42);
  EXPECT_EQ(channel0->get_num_bytes_sent(), 0);
  EXPECT_EQ(channel0->get_num_bytes_received(), sizeof(int));
  EXPECT_EQ(channel1->get_num_bytes_sent(), sizeof(int));
  EXPECT_EQ(channel1->get_num_bytes_received(), 0);
}

TEST_F(CommChannelTest, SendRecvMeasured) {
  SetUp(true);
  int y0, y1;
  {
    boost::thread thread1([this, &y1] {
      int x1 = 42;
      this->channel1->send_recv(x1, y1);
    });
    boost::thread_guard<> g(thread1);
    int x0 = 23;
    channel0->send_recv(x0, y0);
  }
  EXPECT_EQ(y1, 23);
  EXPECT_EQ(y0, 42);
  EXPECT_EQ(channel0->get_num_bytes_sent(), sizeof(int));
  EXPECT_EQ(channel0->get_num_bytes_received(), sizeof(int));
  EXPECT_EQ(channel1->get_num_bytes_sent(), sizeof(int));
  EXPECT_EQ(channel1->get_num_bytes_received(), sizeof(int));
}
