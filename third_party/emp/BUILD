load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")
load("@rules_foreign_cc//tools/build_defs:configure.bzl", "configure_make")

licenses(["notice"])  # MIT

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
    lib_source = "@com_github_emp_toolkit_emp_tool//:all",
    make_commands = [
        "make emp-tool",
        "make install/fast",
    ],
    shared_libraries = [
        "libemp-tool.so",
    ],
    deps = [
        "//third_party/gmp",
        "//third_party/relic",
    ],
)

# We need libcrypto, but we can't include it in the deps to cmake_external due
# to a limitation of rules_foreign_cc. So we wrap both in a cc_library that
# depends on EMP and libcrypto. See also:
# https://github.com/bazelbuild/rules_foreign_cc/issues/232
cc_library(
    name = "emp_tool",
    visibility = ["//visibility:public"],
    deps = [
        ":emp_tool_build",
        "@boost//:system",
        "@boringssl//:crypto",
    ],
)

cmake_external(
    name = "emp_ot_build",
    cache_entries = {
        "CMAKE_PREFIX_PATH": "$EXT_BUILD_DEPS/emp_tool_build",
        "RELIC_INCLUDE_DIR": "$EXT_BUILD_DEPS/relic/include",
        "EMP_USE_RANDOM_DEVICE": "1",
    },
    headers_only = True,
    lib_source = "@com_github_emp_toolkit_emp_ot//:all",
    make_commands = [
        "make install/fast",
    ],
    deps = [
        ":emp_tool_build",
    ],
)

cc_library(
    name = "emp_ot",
    visibility = ["//visibility:public"],
    deps = [
        ":emp_ot_build",
        ":emp_tool",
    ],
)
