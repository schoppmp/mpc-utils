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
   url = "https://github.com/bazelbuild/rules_foreign_cc/archive/42138778e390858c79ea96499e7102d76361f102.zip",
   strip_prefix = "rules_foreign_cc-42138778e390858c79ea96499e7102d76361f102",
   sha256 = "83b7d05b779ff4e04c148d3745824066de03e90f8852c20bc0e576f46bf29139",
)

# Initialize dependencies
load("//mpc-utils:deps.bzl", "mpc_utils_deps")
mpc_utils_deps()
