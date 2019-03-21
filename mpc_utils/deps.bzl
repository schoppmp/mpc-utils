load(
    "@bazel_tools//tools/build_defs/repo:http.bzl",
    "http_archive",
)
load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
load("@com_github_schoppmp_rules_oblivc//oblivc:deps.bzl", "oblivc_deps")
load(
    "@rules_foreign_cc//:workspace_definitions.bzl",
    "rules_foreign_cc_dependencies",
)
load("@mpc_utils//third_party:repo.bzl", "third_party_http_archive")

all_content = """
filegroup(
  name = "all",
  srcs = glob(["**"]),
  visibility = ["//visibility:public"],
)
"""

def mpc_utils_deps(
        enable_oblivc = True):
    # Initialize transitive dependencies.
    boost_deps()
    rules_foreign_cc_dependencies()
    if enable_oblivc:
        oblivc_deps()

    if "org_gmplib" not in native.existing_rules():
        http_archive(
            name = "org_gmplib",
            urls = [
                "https://gmplib.org/download/gmp/gmp-6.1.2.tar.xz",
                "https://ftp.gnu.org/gnu/gmp/gmp-6.1.2.tar.xz",
            ],
            strip_prefix = "gmp-6.1.2",
            build_file_content = all_content,
            sha256 = "87b565e89a9a684fe4ebeeddb8399dce2599f9c9049854ca8c0dfbdea0e21912",
        )

    if "net_shoup_ntl" not in native.existing_rules():
        third_party_http_archive(
            name = "net_shoup_ntl",
            urls = ["https://shoup.net/ntl/ntl-11.3.2.tar.gz"],
            sha256 = "84ba3145abf8d5f3be6832a14c60b3368eb920719ee96e5774587e71ecd66e9d",
            strip_prefix = "ntl-11.3.2",
            # We have to patch  NTL's build file in order to link GMP statically.
            patch_file = "@mpc_utils//third_party/ntl:static_gmp.patch",
            link_files = {
                "@mpc_utils//third_party/ntl:configure": "configure",
            },
            build_file_content = all_content,
        )

    if "googletest" not in native.existing_rules():
        http_archive(
            name = "googletest",
            sha256 = "70404b4a887fd8efce2179e9918e58cdac03245e575408ed87799696e816ecb8",
            strip_prefix = "googletest-f80d6644d4b451f568a2e7aea1e01e842eb242dc",
            urls = ["https://github.com/google/googletest/archive/f80d6644d4b451f568a2e7aea1e01e842eb242dc.zip"],
        )

    if "org_bitbucket_eigen" not in native.existing_rules():
        http_archive(
            name = "org_bitbucket_eigen",
            build_file_content = all_content,
            strip_prefix = "eigen-eigen-323c052e1731",
            urls = ["https://bitbucket.org/eigen/eigen/get/3.3.7.tar.gz"],
            sha256 = "7e84ef87a07702b54ab3306e77cea474f56a40afa1c0ab245bb11725d006d0da",
        )

    if "abseil" not in native.existing_rules():
        http_archive(
            name = "abseil",
            url = "https://github.com/abseil/abseil-cpp/archive/e75672f6afc7e8f23ee7b532e86d1b3b9be3984e.zip",
            strip_prefix = "abseil-cpp-e75672f6afc7e8f23ee7b532e86d1b3b9be3984e",
            sha256 = "ecf95f0fff939cd2741cec1c7baa4bd777eb8b6b8869a47f1343fdc4db86e949",
        )

    if "com_github_gperftools_gperftools" not in native.existing_rules():
        http_archive(
            name = "com_github_gperftools_gperftools",
            sha256 = "1ee8c8699a0eff6b6a203e59b43330536b22bbcbe6448f54c7091e5efb0763c9",
            strip_prefix = "gperftools-2.7",
            urls = ["https://github.com/gperftools/gperftools/releases/download/gperftools-2.7/gperftools-2.7.tar.gz"],
            build_file_content = all_content,
        )
