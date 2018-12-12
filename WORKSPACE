load(
  "@bazel_tools//tools/build_defs/repo:http.bzl",
  "http_archive",
)

http_archive(
    name = "com_github_nelhage_rules_boost",
    url = "https://github.com/nelhage/rules_boost/archive/6d6fd834281cb8f8e758dd9ad76df86304bf1869.zip",
    sha256 = "b4d498a21e9d90ec65720ee3ae4dcbc2f1ce245b2866242514cebbc189d2fc14",
    strip_prefix = "rules_boost-6d6fd834281cb8f8e758dd9ad76df86304bf1869",
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()

http_archive(
    name = "com_github_schoppmp_rules_oblivc",
    sha256 = "9a48460fc805bf552622663db0e895251fa23da24105bc05d3b15c729bf3f1be",
    url = "https://github.com/schoppmp/rules_oblivc/archive/b05c9e8ad842757fd57a7262abfb6b15cbb0f564.zip",
    strip_prefix = "rules_oblivc-b05c9e8ad842757fd57a7262abfb6b15cbb0f564",
)

load("@com_github_schoppmp_rules_oblivc//oblivc:deps.bzl", "oblivc_deps")
oblivc_deps()

http_archive(
    name = "googletest",
    url = "https://github.com/abseil/googletest/archive/6cbd3753dc195595689a0fbb99e7297128a2ed26.zip",
    sha256 = "61598b28fda40dbe0cc0294a61fe0878d4045fa9afe6bdbaf4b3e4f6e1d9a31e",
    strip_prefix = "googletest-6cbd3753dc195595689a0fbb99e7297128a2ed26",
)
