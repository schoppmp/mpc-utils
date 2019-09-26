load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")
load("@rules_foreign_cc//tools/build_defs:configure.bzl", "configure_make")

licenses(["notice"])  # MIT

filegroup(
    name = "all",
    srcs = glob(["**"]),
)

cc_library(
    name = "test_header",
    hdrs = ["test/test.h"],
    include_prefix = "emp-ot",
    visibility = ["//visibility:public"],
)

cmake_external(
    name = "emp_ot_build",
    cache_entries = {
        "CMAKE_PREFIX_PATH": "$EXT_BUILD_DEPS/emp_tool_build;$EXT_BUILD_DEPS/gmp",
        "GMP_INCLUDE_DIR": "$EXT_BUILD_DEPS/gmp/include",
        "OPENSSL_INCLUDE_DIR": "$EXT_BUILD_DEPS/include",
    },
    headers_only = True,
    lib_source = ":all",
    make_commands = [
        "make install/fast",
    ],
    deps = [
        "@boringssl//:ssl",
        "@com_github_emp_toolkit_emp_tool//:emp_tool_build",
        "@mpc_utils//third_party/gmp",
    ],
)

cc_library(
    name = "emp_ot",
    hdrs = glob(["emp-ot/**/*.h"]),
    visibility = ["//visibility:public"],
    deps = [
        ":emp_ot_build",
        "@com_github_emp_toolkit_emp_tool//:emp_tool",
    ],
)
