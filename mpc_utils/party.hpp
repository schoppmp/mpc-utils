#ifndef MPC_UTILS_PARTY_HPP_
#define MPC_UTILS_PARTY_HPP_

#include <mutex>

#include "absl/types/span.h"
#include "boost/asio.hpp"
#include "boost/exception/all.hpp"
#include "boost/thread.hpp"
#include "boost/thread/thread_guard.hpp"
#include "mpc_utils/mpc_config.hpp"

namespace mpc_utils {

// forward_declaration to resolve circular reference between party and
// comm_channel comm_channel.h gets included after declaration of class party
// below
class comm_channel;

// represents a participant in an MPC protocol
class party {
 public:
  static const int DEFAULT_SLEEP_TIME = 500;
  static const int DEFAULT_NUM_TRIES = -1;
  party(mpc_config &conf)
      : servers(conf.servers),
        id(conf.party_id),
        io_thread(boost::bind(&boost::asio::io_service::run, &io_service)),
        resolver(io_service),
        io_guard(io_thread),
        pending(servers.size() + 1){};

  // Establishes a connection to a party with ID party_id and returns a pointer
  // to an object representing that connection.
  comm_channel connect_to(int peer_id, bool measure_communication = false,
                          bool tcp_nodelay = true,
                          int sleep_time = DEFAULT_SLEEP_TIME,
                          int num_tries = DEFAULT_NUM_TRIES,
                          bool create_twin = true);

  // Return this party's ID.
  int get_id() { return id; }

  // Return the number of servers.
  int get_num_servers() { return static_cast<int>(servers.size()); }

  // Returns a view of the servers.
  absl::Span<const server_info> get_servers() const;

  // Error info types for exceptions thrown from class methods
  typedef boost::error_info<struct tag_PEER_ID, int> error_peer_id;
  typedef boost::error_info<struct tag_MY_ID, int> error_my_id;
  typedef boost::error_info<struct tag_NUM_SERVERS, size_t> error_num_servers;

  static const int ANY_PEER = -1;

 private:
  std::vector<server_info> servers;    // list of all servers sorted by party id
  int id;                              // this party's ID
  boost::asio::io_service io_service;  // IO context for this party
  boost::thread io_thread;             // runs io.run()
  boost::asio::ip::tcp::resolver resolver;
  boost::thread_guard<> io_guard;  // joins io_thread whend destroyed
  std::vector<std::vector<comm_channel>> pending;
};

}  // namespace mpc_utils

// TODO: remove this from the global namespace.
using mpc_utils::party;

#endif  // MPC_UTILS_PARTY_HPP_