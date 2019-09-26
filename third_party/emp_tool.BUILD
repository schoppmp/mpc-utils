load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")
load("@rules_foreign_cc//tools/build_defs:configure.bzl", "configure_make")

licenses(["notice"])  # MIT

filegroup(
    name = "all",
    srcs = glob(["**"]),
)

cmake_external(
    name = "emp_tool_build",
    cache_entries = {
        "CMAKE_PREFIX_PATH": "$EXT_BUILD_DEPS/relic;$EXT_BUILD_DEPS/gmp",
        "RELIC_INCLUDE_DIR": "$EXT_BUILD_DEPS/relic/include",
        "GMP_INCLUDE_DIR": "$EXT_BUILD_DEPS/gmp/include",
        "OPENSSL_INCLUDE_DIR": "$EXT_BUILD_DEPS/include",
        "EMP_USE_RANDOM_DEVICE": "1",
    },
    defines = [
        "EMP_USE_RANDOM_DEVICE=1",
    ],
    lib_source = ":all",
    make_commands = [
        "make emp-tool -j $(nproc)",
        "make install/fast",
    ],
    shared_libraries = [
        "libemp-tool.so",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "@boringssl//:ssl",
        "@com_github_relic_toolkit_relic//:relic",
        "@mpc_utils//third_party/gmp",
    ],
)

# We need boost, but we can't include it in the deps to cmake_external due
# to a limitation of rules_foreign_cc. So we wrap both in a cc_library that
# depends on EMP and boost.
cc_library(
    name = "emp_tool",
    hdrs = glob(["emp-tool/**/*.h"]),
    visibility = ["//visibility:public"],
    deps = [
        ":emp_tool_build",
        "@boost//:system",
    ],
)
