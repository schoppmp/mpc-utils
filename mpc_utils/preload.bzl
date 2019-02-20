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
            sha256 = "42331fffb8990f1a8297a213fe74af890935abadd2c68912469f946c63f9622d",
            url = "https://github.com/schoppmp/rules_oblivc/archive/d1a2d3968d4d9387085aed43f5887f44b144901e.zip",
            strip_prefix = "rules_oblivc-d1a2d3968d4d9387085aed43f5887f44b144901e",
        )
    if "rules_foreign_cc" not in native.existing_rules():
        http_archive(
            name = "rules_foreign_cc",
            url = "https://github.com/bazelbuild/rules_foreign_cc/archive/8648b0446092ef2a34d45b02c8dc4c35c3a8df79.zip",
            strip_prefix = "rules_foreign_cc-8648b0446092ef2a34d45b02c8dc4c35c3a8df79",
            sha256 = "20b47a5828d742715e1a03a99036efc5acca927700840a893abc74480e91d4f9",
        )
