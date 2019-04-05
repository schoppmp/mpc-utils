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
        "//mpc_utils/testing:test_deps",
        "@boost//:filesystem",
        "@boost//:program_options",
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
        "//mpc_utils/testing:comm_channel_test_helper",
        "//mpc_utils/testing:test_deps",
        "@abseil//absl/memory",
        "@boost//:thread",
    ],
)

cc_library(
    name = "comm_channel_emp_adapter",
    hdrs = [
        "comm_channel_emp_adapter.hpp",
    ],
    srcs = [
        "comm_channel_emp_adapter.cpp",
    ],
    copts = [
        "-maes",
        "-msse4.2"
    ],
    deps = [
        ":comm_channel",
        ":statusor",
        "//third_party/emp:emp_tool"
    ],
)

cc_test(
    name = "comm_channel_emp_adapter_test",
    srcs = [
        "comm_channel_emp_adapter_test.cpp",
    ],
    copts = [
        "-maes",
        "-msse4.2",
    ],
    deps = [
        ":status_matchers",
        ":comm_channel_emp_adapter",
        "//mpc_utils/testing:comm_channel_test_helper",
        "//mpc_utils/testing:test_deps",
        "//third_party/emp:emp_ot",
        "@com_github_emp_toolkit_emp_ot//:test_header",
    ],
)

cc_library(
    name = "comm_channel_oblivc_adapter",
    srcs = [
        "comm_channel_oblivc_adapter.cpp",
    ],
    hdrs = [
        "comm_channel_oblivc_adapter.hpp",
    ],
    deps = [
        ":statusor",
        ":comm_channel",
        "@oblivc//:runtime",
        "@abseil//absl/strings",
    ],
)

cc_test(
    name = "comm_channel_oblivc_adapter_test",
    size = "small",
    srcs = [
        "comm_channel_oblivc_adapter_test.cpp",
    ],
    deps = [
        ":comm_channel_oblivc_adapter",
        ":mpc_config",
        ":status_matchers",
        "//mpc_utils/testing:comm_channel_test_helper",
        "//mpc_utils/testing:test_deps",
        "@abseil//absl/memory",
        "@boost//:thread",
    ],
)

cc_library(
    name = "openssl_uniform_bit_generator",
    hdrs = [
        "openssl_uniform_bit_generator.hpp",
    ],
    deps = [
        "@boringssl//:crypto",
    ],
)

cc_test(
    name = "openssl_uniform_bit_generator_test",
    size = "small",
    srcs = [
        "openssl_uniform_bit_generator_test.cpp",
    ],
    deps = [
        ":openssl_uniform_bit_generator",
        "//mpc_utils/testing:test_deps",
    ],
)

cc_binary(
    name = "openssl_uniform_bit_generator_benchmark",
    srcs = [
        "openssl_uniform_bit_generator_benchmark.cpp",
    ],
    deps = [
        ":openssl_uniform_bit_generator",
        "@com_github_google_benchmark//:benchmark_main",
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
        "//mpc_utils/testing:test_deps",
        "@abseil//absl/memory",
        "@abseil//absl/strings",
    ],
)
