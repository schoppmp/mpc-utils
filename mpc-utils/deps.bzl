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
        sha256 = "9a48460fc805bf552622663db0e895251fa23da24105bc05d3b15c729bf3f1be",
        url = "https://github.com/schoppmp/rules_oblivc/archive/b05c9e8ad842757fd57a7262abfb6b15cbb0f564.zip",
        strip_prefix = "rules_oblivc-b05c9e8ad842757fd57a7262abfb6b15cbb0f564",
    )

  oblivc_deps()

  if "rules_foreign_cc" not in native.existing_rules():
    http_archive(
       name = "rules_foreign_cc",
       url = "https://github.com/bazelbuild/rules_foreign_cc/archive/b08610b154274940a8b44db53c0f64409fb57878.zip",
       strip_prefix = "rules_foreign_cc-b08610b154274940a8b44db53c0f64409fb57878",
       sha256 = "598951746da89adcaf6cdd99b079ee740edb191bbcb8598b0fe82d5a6c2790e5",
    )

  rules_foreign_cc_dependencies()

  if "org_gmplib" not in native.existing_rules():
    http_archive(
        name = "org_gmplib",
        url = "https://ftp.gnu.org/gnu/gmp/gmp-6.1.2.tar.xz",
        strip_prefix = "gmp-6.1.2",
        build_file_content = all_content,
    )

  if "ntl" not in native.existing_rules():
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
