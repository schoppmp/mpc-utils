#ifndef MPC_UTILS_COMM_CHANNEL_HPP_
#define MPC_UTILS_COMM_CHANNEL_HPP_

#include <iostream>
#include <string>

#include "absl/memory/memory.h"
#include "boost/archive/binary_iarchive.hpp"
#include "boost/archive/binary_oarchive.hpp"
#include "boost/iostreams/filtering_stream.hpp"
#include "boost/thread.hpp"
#include "mpc_utils/party.hpp"

namespace mpc_utils {

// Forward declarations to avoid dependencies on EMP and Obliv-C.
class CommChannelEMPAdapter;
class CommChannelOblivCAdapter;

// Calls `call`, catching exceptions of type `exception_type`, wrapping them
// in a boost::exception and adding the error message of this channel's stream
#define COMM_CHANNEL_WRAP_EXCEPTION(call, exception_type)                      \
  do {                                                                         \
    while (true) {                                                             \
      try {                                                                    \
        call;                                                                  \
        break;                                                                 \
      } catch (exception_type & ex) {                                          \
        if (tcp_stream->error().value() == boost::system::errc::interrupted) { \
          continue; /* Retry on EINTR */                                       \
        } else {                                                               \
          BOOST_THROW_EXCEPTION(boost::enable_error_info(ex) << stream_error(  \
                                    tcp_stream->error().message()));           \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  } while (0)

// A bidirectional connection using the boost serialization library.
class comm_channel {
public:
  comm_channel(std::unique_ptr<boost::asio::ip::tcp::iostream> &&s, party &p,
               int peer_id, bool measure_communication = false);

  // No copy constructor: since copying a channel means establishing a new
  // connection, this should be done explicitly using clone()
  comm_channel(comm_channel &other) = delete;

  // No move constructor. If a comm_channel ever needs to be moved, that should
  // happen using a std::unique_ptr.
  comm_channel(comm_channel &&other) = default;

  // write and read functions for direct binary access
  void write(const char *data, size_t size) {
    COMM_CHANNEL_WRAP_EXCEPTION(oarchive->save_binary(data, size),
                                boost::archive::archive_exception);
    need_flush = true;
  }
  void read(char *buffer, size_t size) {
    flush_if_needed();
    COMM_CHANNEL_WRAP_EXCEPTION(iarchive->load_binary(buffer, size),
                                boost::archive::archive_exception);
  }

  // send and receive objects via boost serialization
  template <typename T> void send(T &&obj) {
    need_flush = true;
    COMM_CHANNEL_WRAP_EXCEPTION(*oarchive & obj,
                                boost::archive::archive_exception);
  }
  template <typename T> void recv(T &&obj) {
    flush_if_needed();
    COMM_CHANNEL_WRAP_EXCEPTION(*iarchive & obj,
                                boost::archive::archive_exception);
  }
  // send from `to_send` and receive into `to_recv` simultaneously
  template <typename S, typename T> void send_recv(S &&to_send, T &&to_recv) {
    if (!twin) {
      twin = absl::WrapUnique(new comm_channel(clone()));
    }
    comm_channel &channel2 = *twin;
    if (get_id() < get_peer_id()) { // lower ID sends on channel2
      boost::thread t([&to_send, &channel2]() {
        channel2.send(to_send);
        channel2.flush();
      });
      boost::thread_guard<> g(t); // join t when leaving scope
      recv(to_recv);
    } else { // higher ID receives on channel2
      boost::thread t([&to_recv, &channel2]() { channel2.recv(to_recv); });
      boost::thread_guard<> g(t); // join t when leaving scope
      send(to_send);
      flush();
    }
  }

  // Flushes all buffers and write outgoing data to the network.
  void flush();

  // ensure everything up to this point was sent/received
  void sync();

  // create a second comm_channel from this one; establishes a new connection
  comm_channel clone();

  // Returns a blocking file descriptor of the underlying socket. Note that any
  // writes that occur directly on this file descriptor will not be measured,
  // even if measure_communication is set to true.
  int get_blocking_fd() {
    tcp_stream->rdbuf()->native_non_blocking(false);
    return static_cast<int>(tcp_stream->rdbuf()->native_handle());
  }

  // getters for both parties' IDs
  int get_id() const { return id; }
  int get_peer_id() const { return peer_id; }

  // Returns whether this channel's communication is measured.
  bool is_measured() const { return measure_communication; }

  // Returns the number of bytes sent so far.
  //
  // Throws std::invalid_argument if measure_communication was not set to true
  // at construction.
  int64_t get_num_bytes_sent() const;

  // Returns the number of bytes received so far.
  //
  // Throws std::invalid_argument if measure_communication was not set to true
  // at construction.
  int64_t get_num_bytes_received() const;

  // Adds a given number of bytes to the internal counter that is returned by
  // get_num_bytes_sent().
  // Should be used to accurately count data sent for example by a cloned
  // channel.
  void add_bytes_sent(int64_t n);

  // Adds a given number of bytes to the internal counter that is returned by
  // get_num_bytes_received().
  // Should be used to accurately count data sent for example by a cloned
  // channel.
  void add_bytes_received(int64_t n);

  // Returns the server_info for the local endpoint.
  server_info get_local_info() const;

  // Returns the server_infor for the remote endpoint.
  server_info get_peer_info() const;

  /**
   * Error info type for exceptions thrown from class methods
   */
  typedef party::error_peer_id error_peer_id;
  typedef party::error_my_id error_my_id;
  typedef party::error_num_servers error_num_servers;
  typedef boost::error_info<struct tag_STREAM_ERROR, std::string> stream_error;

private:
  friend class CommChannelEMPAdapter;
  friend class CommChannelOblivCAdapter;

  party &p;
  int id;
  int peer_id;
  std::unique_ptr<boost::asio::ip::tcp::iostream> tcp_stream;
  std::unique_ptr<boost::iostreams::filtering_ostream> ostream;
  std::unique_ptr<boost::iostreams::filtering_istream> istream;
  std::unique_ptr<boost::archive::binary_oarchive> oarchive;
  std::unique_ptr<boost::archive::binary_iarchive> iarchive;
  std::unique_ptr<comm_channel>
      twin; // used for sending and receiving simultaneously;
  // TODO: somehow allow for simultaneous reading and writing on the _same_
  //   socket
  bool need_flush;
  // If set to true, all send and receive operations will count the number of
  // bytes sent or received, which can be retrieved using
  // get_num_bytes_{sent,received}. Default is false.
  bool measure_communication;
  // Number of bytes for archive headers. Has to be saved for accurate
  // communication measurements. We count both directions to account for
  // possible differences due to boost versions.
  int64_t sent_byte_count, received_byte_count;

  void flush_if_needed() {
    if (need_flush) {
      flush();
    }
  }
};

} // namespace mpc_utils

// TODO: remove this from the global namespace.
using mpc_utils::comm_channel;

#endif // MPC_UTILS_COMM_CHANNEL_HPP_