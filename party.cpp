#include "party.hpp"

comm_channel party::connect_to(int peer_id, bool measure_communication,
                               bool tcp_nodelay, int sleep_time,
                               int num_tries) {
  using tcp = boost::asio::ip::tcp;
  std::unique_lock<std::mutex> lock(connection_mutex);
  try {
    if (peer_id == ANY_PEER) {
      peer_id = servers.size();  // accept any connection
    }
    if (static_cast<unsigned>(peer_id) >= pending.size() || peer_id == id) {
      BOOST_THROW_EXCEPTION(
          boost::enable_error_info(std::invalid_argument("invalid peer_id"))
          << error_num_servers(servers.size()));
    }
    if (!pending[peer_id]
             .empty()) {  // if we already have a connection, return it
      auto chan = std::move(pending[peer_id].back());
      pending[peer_id].pop_back();
      return chan;
    }
    // lower ID listens, higher ID connects.
    if (id < peer_id || peer_id == static_cast<int>(servers.size())) {
      try {
        tcp::endpoint endpoint(tcp::v6(), servers[id].port);
        tcp::acceptor acceptor(io_service, endpoint);
        acceptor.set_option(boost::asio::socket_base::reuse_address(true));
        for (;;) {
          std::unique_ptr<tcp::iostream> stream(new tcp::iostream());
          acceptor.accept(*(stream->rdbuf()));
          if (tcp_nodelay) {
            stream->rdbuf()->set_option(tcp::no_delay(true));  // disable nagle
          }
          comm_channel chan(std::move(stream), *this, -1,
                            measure_communication);
          int current_id = chan.get_peer_id();
          if (current_id > static_cast<int>(servers.size())) {
            current_id = servers.size();  // not a server -> put into last queue
                                          // or return via ANY_PEER'
          }
          if (current_id == peer_id) {  // found the one we're looking for
            return chan;
          }
          if (current_id >= 0 &&
              static_cast<unsigned>(current_id) < pending.size() &&
              current_id != id) {
            pending[current_id].push_back(std::move(
                chan));  // save channel for later; TODO: allow multiple
                         // simultaneous connections from the same peer
          } else {
            // invalid id -> connection gets closed when chan leaves scope
          }
        }
        // wrap exceptions in a boost::exception
      } catch (boost::system::system_error& ex) {
        BOOST_THROW_EXCEPTION(ex);
      } catch (boost::archive::archive_exception& ex) {
        BOOST_THROW_EXCEPTION(ex);
      }
    } else {  // connect
      for (int i = 0; i < num_tries || num_tries == -1; i++) {
        try {
          try {
            std::unique_ptr<tcp::iostream> stream(new tcp::iostream());
            // first, resolve hostname & port
            tcp::resolver::query query(servers[peer_id].host,
                                       std::to_string(servers[peer_id].port));
            // try connecting
            boost::asio::connect(*(stream->rdbuf()), resolver.resolve(query));
            if (tcp_nodelay) {
              stream->rdbuf()->set_option(
                  tcp::no_delay(true));  // disable nagle
            }
            return comm_channel(std::move(stream), *this, peer_id,
                                measure_communication);
            // wrap exceptions in a boost::exception
          } catch (boost::system::system_error& ex) {
            BOOST_THROW_EXCEPTION(ex);
          } catch (boost::archive::archive_exception& ex) {
            BOOST_THROW_EXCEPTION(ex);
          }
        } catch (boost::exception& ex) {
          if (i == num_tries - 1) {  // last try -> re-throw
            throw;
          }
          boost::this_thread::sleep_for(
              boost::chrono::milliseconds(sleep_time));
        }
      }
      BOOST_THROW_EXCEPTION(
          std::runtime_error("Could not connect to server, giving up"));
    }
  } catch (boost::exception& ex) {  // add information to exceptions
    ex << error_my_id(id) << error_peer_id(peer_id);
    throw;
  }
}

#ifdef MPC_UTILS_USE_OBLIVC
int party::connect_to_oblivc(ProtocolDesc& pd, int peer_id,
                             bool measure_communication, int sleep_time,
                             int num_tries) {
  std::unique_lock<std::mutex> lock(connection_mutex);
  // TODO: this currently only works for one simultaneous connection
  if (id < peer_id) {
    std::string port(std::to_string(servers[id].port));
    if (measure_communication) {
      return protocolAcceptTcp2PProfiled(&pd, port.c_str());
    } else {
      return protocolAcceptTcp2P(&pd, port.c_str());
    }
  } else {
    int result = -1;
    for (int i = 0; i < num_tries || num_tries == -1; i++) {
      std::string port(std::to_string(servers[peer_id].port));
      int result;
      if (measure_communication) {
        result = protocolConnectTcp2PProfiled(
            &pd, servers[peer_id].host.c_str(), port.c_str());
      } else {
        result = protocolConnectTcp2P(&pd, servers[peer_id].host.c_str(),
                                      port.c_str());
      }
      if (result != -1) {
        return result;
      }
      boost::this_thread::sleep_for(boost::chrono::milliseconds(sleep_time));
    }
    return result;
  }
}
#endif
