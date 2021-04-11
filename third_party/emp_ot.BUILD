load("@rules_foreign_cc//foreign_cc:cmake.bzl", "cmake")

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

cmake(
    name = "emp_ot",
    cache_entries = {
        "CMAKE_PREFIX_PATH": "$EXT_BUILD_DEPS/emp_tool;$EXT_BUILD_DEPS/gmp",
        "GMP_INCLUDE_DIR": "$EXT_BUILD_DEPS/gmp/include",
    },
    out_headers_only = True,
    lib_source = ":all",
    make_commands = [
        "make install/fast",
    ],
    deps = [
        "@boringssl//:ssl",
        "@com_github_emp_toolkit_emp_tool//:emp_tool",
        "@mpc_utils//third_party/gmp",
    ],
    visibility = ["//visibility:public"],
)
