load(
    "@bazel_tools//tools/build_defs/repo:http.bzl",
    "http_archive",
)

# Dependencies that need to be defined before :deps.bzl can be loaded.
# Copy those in a similar preload.bzl file in any workspace that depends on
# this one.
def mpc_utils_deps_preload():
    if "com_github_nelhage_rules_boost" not in native.existing_rules():
        http_archive(
            name = "com_github_nelhage_rules_boost",
            url = "https://github.com/nelhage/rules_boost/archive/691a53dd7dc4fb8ab70f61acad9b750a1bf10dc6.zip",
            sha256 = "5837d6bcf96c60dc1407126e828287098f91a8c69d8c2ccf8ebb0282ed35b401",
            strip_prefix = "rules_boost-691a53dd7dc4fb8ab70f61acad9b750a1bf10dc6",
        )

    if "com_github_schoppmp_rules_oblivc" not in native.existing_rules():
        http_archive(
            name = "com_github_schoppmp_rules_oblivc",
            sha256 = "0ca82feb4acab59f42ebbf8544c959ef8e9a2a45550c07030c2f0d900c85e185",
            url = "https://github.com/schoppmp/rules_oblivc/archive/08a7ff3b836f14ac45f98eb6abf3004df8b1b59e.zip",
            strip_prefix = "rules_oblivc-08a7ff3b836f14ac45f98eb6abf3004df8b1b59e",
        )

    if "rules_foreign_cc" not in native.existing_rules():
        http_archive(
            name = "rules_foreign_cc",
            url = "https://github.com/bazelbuild/rules_foreign_cc/archive/216ded8acb95d81e312b228dce3c39872c7a7c34.zip",
            strip_prefix = "rules_foreign_cc-216ded8acb95d81e312b228dce3c39872c7a7c34",
            sha256 = "bb38d30c5d06cc1aedc9db7d2274d2323419a60200ac8e5fdbdc100e37740975",
        )
