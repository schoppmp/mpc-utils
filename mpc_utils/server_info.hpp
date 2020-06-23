#pragma once
#include <string>

namespace mpc_utils {

struct server_info {
  std::string host;
  uint16_t port;

  server_info(const std::string &host, uint16_t port)
      : host(host), port(port) {}

  int compare(const server_info &other) const;
  bool operator==(const server_info &other) const {
    return (compare(other) == 0);
  };
  bool operator!=(const server_info &other) const {
    return (compare(other) != 0);
  };
  bool operator<=(const server_info &other) const {
    return (compare(other) <= 0);
  };
  bool operator>=(const server_info &other) const {
    return (compare(other) >= 0);
  };
  bool operator>(const server_info &other) const {
    return (compare(other) > 0);
  };
  bool operator<(const server_info &other) const {
    return (compare(other) < 0);
  };
};

} // namespace mpc_utils

using mpc_utils::server_info;

std::ostream &operator<<(std::ostream &stream,
                         const mpc_utils::server_info &server);
