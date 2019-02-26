cc_library(
    name = "comm_channel_test_helper",
    srcs = [
        "comm_channel_test_helper.cpp",
    ],
    hdrs = [
        "comm_channel_test_helper.hpp",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//mpc_utils:comm_channel",
        "@abseil//absl/memory",
        "@boost//:thread",
    ],
)

cc_library(
    name = "comm_channel_test_helper_oblivc",
    srcs = [
        "comm_channel_test_helper.cpp",
    ],
    hdrs = [
        "comm_channel_test_helper.hpp",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//mpc_utils:comm_channel_oblivc",
        "@abseil//absl/memory",
        "@boost//:thread",
        "@oblivc//:runtime",
    ],
)

cc_library(
    name = "leack_check",
    visibility = ["//visibility:public"],
    data = [
      "@mpc_utils//third_party/gperftools:pprof"
    ],
    deps = [
        "//third_party/gperftools",
    ],
)

cc_library(
    name = "test_deps",
    visibility = ["//visibility:public"],
    deps = [
        "@googletest//:gtest_main",
        ":leak_check",
    ],
)