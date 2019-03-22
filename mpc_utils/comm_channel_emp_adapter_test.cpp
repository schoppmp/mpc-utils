#include "mpc_utils/comm_channel_emp_adapter.hpp"
#include <iostream>
#include <random>
#include "boost/thread.hpp"
#include "boost/thread/thread_guard.hpp"
#include "gtest/gtest.h"
#include "mpc_utils/testing/comm_channel_test_helper.hpp"
#include "test/test.h"

namespace mpc_utils {
namespace {

void RunParty(int party, testing::CommChannelTestHelper *helper) {
  int length = 1 << 20;
  int ot_party = party == 0 ? ALICE : BOB;
  CommChannelEMPAdapter io(helper->GetChannel(party));
  std::cout << "NPOT\t"
            << 10000.0 /
                   test_ot<CommChannelEMPAdapter, OTNP>(&io, ot_party, 10000) *
                   1e6
            << " OTps" << endl;
  std::cout << "Semi Honest OT Extension\t"
            << double(length) /
                   test_ot<CommChannelEMPAdapter, SHOTExtension>(&io, ot_party,
                                                                 length) *
                   1e6
            << " OTps" << endl;
  std::cout << "Semi Honest COT Extension\t"
            << double(length) /
                   test_cot<CommChannelEMPAdapter, SHOTExtension>(&io, ot_party,
                                                                  length) *
                   1e6
            << " OTps" << endl;
  std::cout << "Semi Honest ROT Extension\t"
            << double(length) /
                   test_rot<CommChannelEMPAdapter, SHOTExtension>(&io, ot_party,
                                                                  length) *
                   1e6
            << " OTps" << endl;
}

TEST(EmpOt, TestOt) {
  testing::CommChannelTestHelper helper(false);
  boost::thread thread1([&helper] { RunParty(0, &helper); });
  boost::thread_guard<> g(thread1);
  RunParty(1, &helper);
}

}  // namespace

}  // namespace mpc_utils