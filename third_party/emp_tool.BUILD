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
        "RELIC_INCLUDE_DIR": "$EXT_BUILD_DEPS/relic/include",
        "CMAKE_PREFIX_PATH": "$EXT_BUILD_DEPS/relic",
        "EMP_USE_RANDOM_DEVICE": "1",
    },
    defines = [
        "EMP_USE_RANDOM_DEVICE=1",
    ],
    lib_source = ":all",
    make_commands = [
        "make emp-tool",
        "make install/fast",
    ],
    shared_libraries = [
        "libemp-tool.so",
    ],
    deps = [
        "@mpc_utils//third_party/gmp",
        "@com_github_relic_toolkit_relic//:relic",
    ],
    visibility = ["//visibility:public"],
)

# We need libcrypto, but we can't include it in the deps to cmake_external due
# to a limitation of rules_foreign_cc. So we wrap both in a cc_library that
# depends on EMP and libcrypto. See also:
# https://github.com/bazelbuild/rules_foreign_cc/issues/232
cc_library(
    name = "emp_tool",
    hdrs = glob(["emp-tool/**/*.h"]),
    deps = [
        ":emp_tool_build",
        "@boost//:system",
        "@boringssl//:crypto",
    ],
    visibility = ["//visibility:public"],
)
