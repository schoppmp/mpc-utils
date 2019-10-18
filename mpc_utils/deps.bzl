load(
    "@bazel_tools//tools/build_defs/repo:http.bzl",
    "http_archive",
)
load("@mpc_utils//third_party:repo.bzl", "third_party_http_archive")
load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
load("@com_github_schoppmp_rules_oblivc//oblivc:deps.bzl", "oblivc_deps")
load(
    "@rules_foreign_cc//:workspace_definitions.bzl",
    "rules_foreign_cc_dependencies",
)

all_content = """
filegroup(
  name = "all",
  srcs = glob(["**"]),
  visibility = ["//visibility:public"],
)
"""

def clean_dep(dep):
    return str(Label(dep))

def mpc_utils_deps(
        enable_oblivc = True,
        enable_serialization = True,
        enable_emp = True):
    # Initialize transitive dependencies.
    boost_deps()

    # Initialize optional dependencies
    if enable_oblivc:
        oblivc_deps()
    if enable_serialization:
        rules_foreign_cc_dependencies()
        if "org_bitbucket_eigen" not in native.existing_rules():
            http_archive(
                name = "org_bitbucket_eigen",
                build_file_content = all_content,
                strip_prefix = "eigen-eigen-323c052e1731",
                urls = ["https://bitbucket.org/eigen/eigen/get/3.3.7.tar.gz"],
                sha256 = "7e84ef87a07702b54ab3306e77cea474f56a40afa1c0ab245bb11725d006d0da",
            )
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
                urls = ["https://shoup.net/ntl/ntl-11.3.4.tar.gz"],
                sha256 = "2d93baa2a1c6c7477113dc71413dedf74bcc8d9477427c3d1097be68fa30be39",
                strip_prefix = "ntl-11.3.4",
                # We have to patch  NTL's build file in order to link GMP statically.
                patch_file = clean_dep("//third_party/ntl:static_gmp.patch"),
                link_files = {
                    clean_dep("//third_party/ntl:configure"): "configure",
                },
                build_file_content = all_content,
            )
    if enable_emp:
        if "com_github_relic_toolkit_relic" not in native.existing_rules():
            http_archive(
                name = "com_github_relic_toolkit_relic",
                url = "https://github.com/relic-toolkit/relic/archive/f6fe4bad7739e67d1de6ec308f350678cbe5746f.zip",
                sha256 = "e6e75e077006b169d25af44c7e2f00b6e43495c7259a0433ba8cd786a55060fe",
                strip_prefix = "relic-f6fe4bad7739e67d1de6ec308f350678cbe5746f",
                build_file = clean_dep("//third_party:relic.BUILD"),
            )
        if "com_github_emp_toolkit_emp_tool" not in native.existing_rules():
            third_party_http_archive(
                name = "com_github_emp_toolkit_emp_tool",
                url = "https://github.com/emp-toolkit/emp-tool/archive/50c01ba99e5d257de05ef0e74ce6a0294a9ff471.zip",
                sha256 = "6f90e194a2f709f51c5082a333fa070983c822236bb6a48c046fdd8b84cc80fe",
                strip_prefix = "emp-tool-50c01ba99e5d257de05ef0e74ce6a0294a9ff471",
                build_file = clean_dep("//third_party:emp_tool.BUILD"),
                link_files = {
                    clean_dep("//third_party/emp-tool:FindBoost.cmake"): "cmake/FindBoost.cmake",
                },
            )
        if "com_github_emp_toolkit_emp_ot" not in native.existing_rules():
            http_archive(
                name = "com_github_emp_toolkit_emp_ot",
                url = "https://github.com/emp-toolkit/emp-ot/archive/15fb731e528974bcfe5aa09c18bb16376e949283.zip",
                sha256 = "aa8b4f773c0e7297709453283a4744a47907762fcdd122b26c594410b1c50ce6",
                strip_prefix = "emp-ot-15fb731e528974bcfe5aa09c18bb16376e949283",
                build_file = clean_dep("//third_party:emp_ot.BUILD"),
            )

    # Other dependencies.
    if "googletest" not in native.existing_rules():
        http_archive(
            name = "googletest",
            sha256 = "89e98c265b80181d902b1a19c10c29b3a22d804b207214d8104ad42905fbae87",
            strip_prefix = "googletest-f2fb48c3b3d79a75a88a99fba6576b25d42ec528",
            urls = ["https://github.com/google/googletest/archive/f2fb48c3b3d79a75a88a99fba6576b25d42ec528.zip"],
        )
    if "com_github_gperftools_gperftools" not in native.existing_rules():
        http_archive(
            name = "com_github_gperftools_gperftools",
            sha256 = "1ee8c8699a0eff6b6a203e59b43330536b22bbcbe6448f54c7091e5efb0763c9",
            strip_prefix = "gperftools-2.7",
            urls = ["https://github.com/gperftools/gperftools/releases/download/gperftools-2.7/gperftools-2.7.tar.gz"],
            build_file_content = all_content,
        )
    if "com_github_google_benchmark" not in native.existing_rules():
        http_archive(
            name = "com_github_google_benchmark",
            url = "https://github.com/google/benchmark/archive/b874e72208b6e21b62287942e5e3b11f6630107f.zip",
            sha256 = "25f331d18e1f3065dbfac5b1aae4c55480e556f801259130be0d8fdd158a6b46",
            strip_prefix = "benchmark-b874e72208b6e21b62287942e5e3b11f6630107f",
        )
    if "boringssl" not in native.existing_rules():
        http_archive(
            name = "boringssl",
            sha256 = "641bc564e98467dcef96a5089ece9ac22728ed03695def77722cc41b8997a660",
            strip_prefix = "boringssl-532d22c4b4f6a13fa9bcf0be9510b3b7a4685898",
            urls = [
                "https://github.com/google/boringssl/archive/532d22c4b4f6a13fa9bcf0be9510b3b7a4685898.zip",
            ],
        )
