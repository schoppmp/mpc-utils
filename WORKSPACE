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
    sha256 = "1b7499c1c08058ce525918e556253863791eeebf268b5c403cef250fb3010aae",
    url = "https://github.com/schoppmp/rules_oblivc/archive/3e6ac74e027d11f1dbc4d110358ed7cf803e6a3d.zip",
    strip_prefix = "rules_oblivc-3e6ac74e027d11f1dbc4d110358ed7cf803e6a3d",
)

http_archive(
   name = "rules_foreign_cc",
   url = "https://github.com/bazelbuild/rules_foreign_cc/archive/99ea1b09fc3fe9a1fbb32d965324d9dc34c31cde.zip",
   strip_prefix = "rules_foreign_cc-99ea1b09fc3fe9a1fbb32d965324d9dc34c31cde",
   sha256 = "5ffb7001ae6fe455a1b7baceb0944112183d3523efcdc0a674a964837d4e7312",
)

# Initialize dependencies
load("//mpc-utils:deps.bzl", "mpc_utils_deps")
mpc_utils_deps()
