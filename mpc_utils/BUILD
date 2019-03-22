package(default_visibility = ["//visibility:public"])

cc_library(
    name = "benchmarker",
    srcs = ["benchmarker.cpp"],
    hdrs = ["benchmarker.hpp"],
)

cc_test(
    name = "benchmarker_test",
    size = "small",
    srcs = ["benchmarker_test.cpp"],
    deps = [
        ":benchmarker",
        "//mpc_utils/testing:test_deps",
    ],
)

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
        "@boost//:exception",
        "@boost//:program_options",
    ],
)

cc_test(
    name = "config_test",
    srcs = ["config_test.cpp"],
    deps = [
        ":config",
        "@boost//:filesystem",
        "@boost//:program_options",
        "//mpc_utils/testing:test_deps",
    ],
)

cc_library(
    name = "mpc_config",
    srcs = ["mpc_config.cpp"],
    hdrs = ["mpc_config.hpp"],
    deps = [
        ":config",
        ":server_info",
    ],
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
        "@abseil//absl/memory",
        "@boost//:asio",
        "@boost//:exception",
        "@boost//:iostreams",
        "@boost//:serialization",
        "@boost//:thread",
    ],
)

cc_test(
    name = "comm_channel_test",
    size = "small",
    srcs = [
        "comm_channel_test.cpp",
    ],
    deps = [
        ":comm_channel",
        ":mpc_config",
        "@abseil//absl/memory",
        "@boost//:thread",
        "//mpc_utils/testing:comm_channel_test_helper",
        "//mpc_utils/testing:test_deps",
    ],
)

cc_library(
    name = "comm_channel_emp_adapter",
    hdrs = [
        "comm_channel_emp_adapter.hpp",
    ],
    deps = [
        ":comm_channel",
        "//third_party/emp:emp_tool"
    ]
)

cc_test(
    name = "comm_channel_emp_adapter_test",
    srcs = [
        "comm_channel_emp_adapter_test.cpp",
    ],
    copts = [
        "-maes",
        "-msse4.2"
    ],
    deps = [
        ":comm_channel_emp_adapter",
        "//mpc_utils/testing:test_deps",
        "//mpc_utils/testing:comm_channel_test_helper",
        "@com_github_emp_toolkit_emp_ot//:test_header",
        "//third_party/emp:emp_ot"
    ],
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
    defines = [
        "MPC_UTILS_USE_OBLIVC",
    ],
    deps = [
        ":mpc_config",
        "@abseil//absl/memory",
        "@boost//:asio",
        "@boost//:exception",
        "@boost//:iostreams",
        "@boost//:serialization",
        "@boost//:thread",
        "@oblivc//:runtime",
    ],
)

cc_test(
    name = "comm_channel_oblivc_test",
    size = "small",
    srcs = [
        "comm_channel_oblivc_test.cpp",
    ],
    deps = [
        ":comm_channel_oblivc",
        ":mpc_config",
        "@abseil//absl/memory",
        "@boost//:thread",
        "//mpc_utils/testing:comm_channel_test_helper_oblivc",
        "//mpc_utils/testing:test_deps",
    ],
)

cc_library(
    name = "status",
    srcs = [
        "canonical_errors.cc",
        "status.cc",
    ],
    hdrs = [
        "canonical_errors.h",
        "status.h",
        "status_internal.h",
        "status_macros.h",
    ],
    deps = [
        "@abseil//absl/base:core_headers",
        "@abseil//absl/meta:type_traits",
        "@abseil//absl/strings",
    ],
)

cc_test(
    name = "status_test",
    srcs = ["status_test.cc"],
    deps = [
        ":status_matchers",
        "//mpc_utils/testing:test_deps",
    ],
)

cc_library(
    name = "statusor",
    hdrs = ["statusor.h"],
    deps = [
        ":status",
        "@abseil//absl/base",
        "@abseil//absl/base:core_headers",
        "@abseil//absl/types:variant",
    ],
)

cc_library(
    name = "status_matchers",
    testonly = 1,
    hdrs = ["status_matchers.h"],
    deps = [
        ":status",
        ":statusor",
        "@abseil//absl/types:optional",
        "@googletest//:gtest",
    ],
)

cc_test(
    name = "statusor_test",
    srcs = ["statusor_test.cc"],
    deps = [
        ":status",
        ":status_matchers",
        ":statusor",
        "//mpc_utils/testing:test_deps",
    ],
)

cc_test(
    name = "status_macros_test",
    srcs = ["status_macros_test.cc"],
    deps = [
        ":status",
        ":status_matchers",
        ":statusor",
        "@abseil//absl/memory",
        "@abseil//absl/strings",
        "//mpc_utils/testing:test_deps",
    ],
)