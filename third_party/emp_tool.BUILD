load("@rules_foreign_cc//foreign_cc:cmake.bzl", "cmake")

licenses(["notice"])  # MIT

filegroup(
    name = "all",
    srcs = glob(["**"]),
)

cmake(
    name = "emp_tool",
    cache_entries = {
        "CMAKE_PREFIX_PATH": "$EXT_BUILD_DEPS;$EXT_BUILD_DEPS/relic;$EXT_BUILD_DEPS/gmp",
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
    out_shared_libs = [
        "libemp-tool.so",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "@boringssl//:ssl",
        "@boringssl//:crypto",
        "@com_github_relic_toolkit_relic//:relic",
        "@mpc_utils//third_party/gmp",
        "@boost//:system",
    ],
)
