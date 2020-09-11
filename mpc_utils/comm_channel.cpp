#include "mpc_utils/comm_channel.hpp"

#include "absl/memory/memory.h"
#include "mpc_utils/counter.hpp"

namespace mpc_utils {

// constructor called from party::connect_to
comm_channel::comm_channel(std::unique_ptr<boost::asio::ip::tcp::iostream> &&s,
                           party &p, int peer_id, bool measure_communication,
                           std::unique_ptr<comm_channel> twin)
    : p(p),
      id(p.get_id()),
      tcp_stream(std::move(s)),
      twin(std::move(twin)),
      need_flush(false),
      measure_communication(measure_communication),
      sent_byte_count(0),
      received_byte_count(0) {
  // Build output archive, optionally measuring communication.
  if (!measure_communication) {
    oarchive = absl::make_unique<boost::archive::binary_oarchive>(
        *tcp_stream, boost::archive::no_codecvt);
  } else {
    ostream = absl::make_unique<boost::iostreams::filtering_ostream>();
    ostream->push(counter(), 0, 0);
    ostream->push(*tcp_stream, 0, 0);
    oarchive = absl::make_unique<boost::archive::binary_oarchive>(
        *ostream, boost::archive::no_codecvt);
  }

  // Flush so that headers get written to the network that need to be read when
  // the other party constructs their input archive.
  flush();

  // Build input archive, optionally measuring communication.
  if (!measure_communication) {
    iarchive = absl::make_unique<boost::archive::binary_iarchive>(
        *tcp_stream, boost::archive::no_codecvt);
  } else {
    istream = absl::make_unique<boost::iostreams::filtering_istream>();
    istream->push(counter(), 0, 0);
    istream->push(*tcp_stream, 0, 0);
    iarchive = absl::make_unique<boost::archive::binary_iarchive>(
        *istream, boost::archive::no_codecvt);
  }

  if (peer_id == -1) {
    recv(peer_id);  // read id of remote
  } else {
    send(this->id);  // send own ID to remote
    flush();
  }

  if (measure_communication) {
    // Subtract header from bytes sent/received.
    sent_byte_count = -ostream->component<counter>(0)->characters();
    received_byte_count = -istream->component<counter>(0)->characters();
  }

  if (peer_id < 0 || peer_id == id) {
    BOOST_THROW_EXCEPTION(
        boost::enable_error_info(std::invalid_argument("invalid peer_id"))
        << error_num_servers(p.get_num_servers()) << error_my_id(id)
        << error_peer_id(peer_id));
  }
  this->peer_id = peer_id;
};

// create a second comm_channel from this one; establishes a new connection
comm_channel comm_channel::clone() {
  boost::asio::ip::tcp::no_delay no_delay;
  tcp_stream->rdbuf()->get_option(no_delay);
  flush_if_needed();
  return p.connect_to(peer_id, measure_communication, no_delay.value());
}

// flush underlying stream
void comm_channel::flush() {
  if (measure_communication) {
    COMM_CHANNEL_WRAP_EXCEPTION(ostream->strict_sync(), std::ios_base::failure);
  }
  // strict_sync does not seem to flush tcp_stream, so we do it here even if
  // measure_communication is true.
  COMM_CHANNEL_WRAP_EXCEPTION(tcp_stream->flush(), std::ios_base::failure);
  need_flush = false;
}

void comm_channel::sync() {
  int a = 0, b;
  send_recv(a, b);
}

int64_t comm_channel::get_num_bytes_sent() const {
  if (!measure_communication) {
    BOOST_THROW_EXCEPTION(std::invalid_argument(
        "get_num_bytes_sent may only be called if measure_communication = true "
        "was passed at construction"));
  }
  int64_t sent = ostream->component<counter>(0)->characters() + sent_byte_count;
  if (twin) {
    sent += twin->get_num_bytes_sent();
  }
  return sent;
}

int64_t comm_channel::get_num_bytes_received() const {
  if (!measure_communication) {
    BOOST_THROW_EXCEPTION(std::invalid_argument(
        "get_num_bytes_received may only be called if measure_communication = "
        "true was passed at construction"));
  }
  int64_t received =
      istream->component<counter>(0)->characters() + received_byte_count;
  if (twin) {
    received += twin->get_num_bytes_received();
  }
  return received;
}

void comm_channel::add_bytes_sent(int64_t n) { sent_byte_count += n; }

void comm_channel::add_bytes_received(int64_t n) { received_byte_count += n; }

server_info comm_channel::get_local_info() const { return p.get_servers()[id]; }

server_info comm_channel::get_peer_info() const {
  return p.get_servers()[peer_id];
}

}  // namespace mpc_utils