load(
  "@bazel_tools//tools/build_defs/repo:http.bzl",
  "http_archive",
)
load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
load("@com_github_schoppmp_rules_oblivc//oblivc:deps.bzl", "oblivc_deps")
load(
  "@rules_foreign_cc//:workspace_definitions.bzl",
  "rules_foreign_cc_dependencies"
)

all_content = """
filegroup(
  name = "all",
  srcs = glob(["**"]),
  visibility = ["//visibility:public"]
)
"""

def mpc_utils_deps():
  if "com_github_nelhage_rules_boost" not in native.existing_rules():
    native.http_archive(
        name = "com_github_nelhage_rules_boost",
        url = "https://github.com/nelhage/rules_boost/archive/6d6fd834281cb8f8e758dd9ad76df86304bf1869.zip",
        sha256 = "b4d498a21e9d90ec65720ee3ae4dcbc2f1ce245b2866242514cebbc189d2fc14",
        strip_prefix = "rules_boost-6d6fd834281cb8f8e758dd9ad76df86304bf1869",
    )

  boost_deps()

  if "com_github_schoppmp_rules_oblivc" not in native.existing_rules():
    http_archive(
        name = "com_github_schoppmp_rules_oblivc",
        sha256 = "1b7499c1c08058ce525918e556253863791eeebf268b5c403cef250fb3010aae",
        url = "https://github.com/schoppmp/rules_oblivc/archive/3e6ac74e027d11f1dbc4d110358ed7cf803e6a3d.zip",
        strip_prefix = "rules_oblivc-3e6ac74e027d11f1dbc4d110358ed7cf803e6a3d",
    )

  oblivc_deps()

  if "rules_foreign_cc" not in native.existing_rules():
    http_archive(
       name = "rules_foreign_cc",
       url = "https://github.com/bazelbuild/rules_foreign_cc/archive/42138778e390858c79ea96499e7102d76361f102.zip",
       strip_prefix = "rules_foreign_cc-42138778e390858c79ea96499e7102d76361f102",
       sha256 = "83b7d05b779ff4e04c148d3745824066de03e90f8852c20bc0e576f46bf29139",
    )

  rules_foreign_cc_dependencies()

  if "org_gmplib" not in native.existing_rules():
    http_archive(
        name = "org_gmplib",
        url = "https://ftp.gnu.org/gnu/gmp/gmp-6.1.2.tar.xz",
        strip_prefix = "gmp-6.1.2",
        build_file_content = all_content,
        sha256 = "87b565e89a9a684fe4ebeeddb8399dce2599f9c9049854ca8c0dfbdea0e21912",
    )

  if "net_shoup_ntl" not in native.existing_rules():
    http_archive(
        name = "net_shoup_ntl",
        url = "https://shoup.net/ntl/ntl-11.3.2.tar.gz",
        sha256 = "84ba3145abf8d5f3be6832a14c60b3368eb920719ee96e5774587e71ecd66e9d",
        strip_prefix = "ntl-11.3.2",
        build_file_content = all_content,
    )

  if "googletest" not in native.existing_rules():
    http_archive(
        name = "googletest",
        url = "https://github.com/abseil/googletest/archive/6cbd3753dc195595689a0fbb99e7297128a2ed26.zip",
        sha256 = "61598b28fda40dbe0cc0294a61fe0878d4045fa9afe6bdbaf4b3e4f6e1d9a31e",
        strip_prefix = "googletest-6cbd3753dc195595689a0fbb99e7297128a2ed26",
    )

  if "com_github_xianyi_openblas" not in native.existing_rules():
    http_archive(
      name = "com_github_xianyi_openblas",
      build_file_content = all_content,
      strip_prefix = "OpenBLAS-0.3.4",
      urls = ["https://github.com/xianyi/OpenBLAS/archive/v0.3.4.tar.gz"],
      sha256 = "4b4b4453251e9edb5f57465bf2b3cf67b19d811d50c8588cdf2ea1f201bb834f",
    )

  if "org_bitbucket_eigen" not in native.existing_rules():
    http_archive(
      name = "org_bitbucket_eigen",
      build_file_content = all_content,
      strip_prefix = "eigen-eigen-323c052e1731",
      urls = ["https://bitbucket.org/eigen/eigen/get/3.3.7.tar.gz"],
      sha256 = "7e84ef87a07702b54ab3306e77cea474f56a40afa1c0ab245bb11725d006d0da",
    )
