cc_library(
    name = "comm_channel_oblivc_test_helper",
    srcs = [
        "comm_channel_oblivc_test_helper.cpp",
    ],
    hdrs = [
        "comm_channel_oblivc_test_helper.hpp",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//mpc_utils:comm_channel_oblivc",
        "@abseil//absl/memory",
        "@boost//:thread",
        "@googletest//:gtest",
        "@oblivc//:runtime",
    ],
)
