cc_library(
  name = "server_info",
  srcs = ["server_info.cpp"],
  hdrs = ["server_info.hpp"],
)

cc_library(
  name = "config",
  srcs = ["config.cpp"],
  hdrs = ["config.hpp"],
  deps = [
    ":server_info",
    "@boost//:program_options",
    "@boost//:exception",
  ]
)

cc_library(
  name = "mpc_config",
  srcs = ["mpc_config.cpp"],
  hdrs = ["mpc_config.hpp"],
  deps = [
    ":config",
    ":server_info",
  ]
)

cc_library(
  name = "comm_channel",
  srcs = [
    "comm_channel.cpp",
    "party.cpp",
  ],
  hdrs = [
    "comm_channel.hpp",
    "party.hpp",
  ],
  deps = [
    ":mpc_config",
    "@boost//:exception",
    "@boost//:asio",
    "@boost//:thread",
    "@boost//:serialization",
  ]
)

cc_library(
  name = "comm_channel_oblivc",
  srcs = [
    "comm_channel.cpp",
    "party.cpp",
  ],
  hdrs = [
    "comm_channel.hpp",
    "party.hpp",
  ],
  copts = [
    "-DMPC_UTILS_USE_OBLIVC",
  ],
  deps = [
    ":mpc_config",
    "@boost//:exception",
    "@boost//:asio",
    "@boost//:thread",
    "@boost//:serialization",
    "@io_rules_oblivc//oblivc",
  ]
)
