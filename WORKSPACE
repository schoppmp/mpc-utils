load(
  "@bazel_tools//tools/build_defs/repo:git.bzl",
  "git_repository",
)

git_repository(
    name = "com_github_nelhage_rules_boost",
    commit = "17d9bf68420ec2e4c63ba85421e9959f52f977aa",
    remote = "https://github.com/nelhage/rules_boost",
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()

git_repository(
    name = "io_rules_oblivc",
    commit = "19d5311181a1082b122a61c396b2db9620b5e196",
    remote = "https://github.com/schoppmp/rules_oblivc",
)

load("@io_rules_oblivc//oblivc:deps.bzl", "oblivc_deps")

oblivc_deps()
