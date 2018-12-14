load(
  "@bazel_tools//tools/build_defs/repo:http.bzl",
  "http_archive",
)

# Transitive dependencies needed for mpc_utils_deps()
http_archive(
    name = "com_github_nelhage_rules_boost",
    url = "https://github.com/nelhage/rules_boost/archive/6d6fd834281cb8f8e758dd9ad76df86304bf1869.zip",
    sha256 = "b4d498a21e9d90ec65720ee3ae4dcbc2f1ce245b2866242514cebbc189d2fc14",
    strip_prefix = "rules_boost-6d6fd834281cb8f8e758dd9ad76df86304bf1869",
)

http_archive(
    name = "com_github_schoppmp_rules_oblivc",
    sha256 = "9a48460fc805bf552622663db0e895251fa23da24105bc05d3b15c729bf3f1be",
    url = "https://github.com/schoppmp/rules_oblivc/archive/b05c9e8ad842757fd57a7262abfb6b15cbb0f564.zip",
    strip_prefix = "rules_oblivc-b05c9e8ad842757fd57a7262abfb6b15cbb0f564",
)

http_archive(
   name = "rules_foreign_cc",
   url = "https://github.com/bazelbuild/rules_foreign_cc/archive/b08610b154274940a8b44db53c0f64409fb57878.zip",
   strip_prefix = "rules_foreign_cc-b08610b154274940a8b44db53c0f64409fb57878",
   sha256 = "598951746da89adcaf6cdd99b079ee740edb191bbcb8598b0fe82d5a6c2790e5",
)

# Initialize dependencies
load("//mpc-utils:deps.bzl", "mpc_utils_deps")
mpc_utils_deps()
