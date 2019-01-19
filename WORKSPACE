workspace(name = "com_github_schoppmp_mpc_utils")

load(
    "@bazel_tools//tools/build_defs/repo:http.bzl",
    "http_archive",
)

# Initialize dependencies
load("//mpc_utils:preload.bzl", "mpc_utils_deps_preload")

mpc_utils_deps_preload()

load("//mpc_utils:deps.bzl", "mpc_utils_deps")

mpc_utils_deps()
