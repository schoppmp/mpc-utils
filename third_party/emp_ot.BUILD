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
        "CMAKE_PREFIX_PATH": "$EXT_BUILD_DEPS/emp_tool_build",
        "RELIC_INCLUDE_DIR": "$EXT_BUILD_DEPS/relic/include",
        "EMP_USE_RANDOM_DEVICE": "1",
    },
    headers_only = True,
    lib_source = ":all",
    make_commands = [
        "make install/fast",
    ],
    deps = [
        "@com_github_emp_toolkit_emp_tool//:emp_tool_build",
    ],
)

cc_library(
    name = "emp_ot",
    hdrs = glob(["emp-ot/**/*.h"]),
    deps = [
        ":emp_ot_build",
        "@com_github_emp_toolkit_emp_tool//:emp_tool",
    ],
    visibility = ["//visibility:public"],
)
