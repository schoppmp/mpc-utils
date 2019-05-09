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
        "//mpc_utils:openssl_uniform_bit_generator",
        "@abseil//absl/memory",
        "@boost//:thread",
    ],
)

cc_library(
    name = "leak_check",
    data = [
        "//third_party/gperftools:pprof",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//third_party/gperftools",
    ],
)

cc_library(
    name = "test_deps",
    visibility = ["//visibility:public"],
    deps = [
        ":leak_check",
        "@googletest//:gtest_main",
    ],
)
