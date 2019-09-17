load(
    "@bazel_tools//tools/build_defs/repo:http.bzl",
    "http_archive",
)

# Dependencies that need to be defined before :deps.bzl can be loaded.
# Copy those in a similar preload.bzl file in any workspace that depends on
# this one.
def mpc_utils_deps_preload():
    if "com_google_absl" not in native.existing_rules():
        http_archive(
            name = "com_google_absl",
            urls = ["https://github.com/abseil/abseil-cpp/archive/7c7754fb3ed9ffb57d35fe8658f3ba4d73a31e72.zip"],  # 2019-03-14
            strip_prefix = "abseil-cpp-7c7754fb3ed9ffb57d35fe8658f3ba4d73a31e72",
            sha256 = "71d00d15fe6370220b6685552fb66e5814f4dd2e130f3836fc084c894943753f",
        )
    if "com_github_nelhage_rules_boost" not in native.existing_rules():
        http_archive(
            name = "com_github_nelhage_rules_boost",
            url = "https://github.com/nelhage/rules_boost/archive/417642961150e987bc1ac78c7814c617566ffdaa.zip",
            sha256 = "b24dd149c0cc9f7ff5689d91f99aaaea0d340baf9911439b573f02074148533a",
            strip_prefix = "rules_boost-417642961150e987bc1ac78c7814c617566ffdaa",
        )
    if "com_github_schoppmp_rules_oblivc" not in native.existing_rules():
        http_archive(
            name = "com_github_schoppmp_rules_oblivc",
            sha256 = "bd338f66667cc959d32ed9a048b7d0b9d3d8f554fd36a7bc6b7102a4a355ab30",
            url = "https://github.com/schoppmp/rules_oblivc/archive/99aeebf6732f209aa85af8b069220e18082b92fe.zip",
            strip_prefix = "rules_oblivc-99aeebf6732f209aa85af8b069220e18082b92fe",
        )
    if "rules_foreign_cc" not in native.existing_rules():
        http_archive(
            name = "rules_foreign_cc",
            url = "https://github.com/bazelbuild/rules_foreign_cc/archive/8ccd83504b2221b670fc0b83d78fcee5642f4cb1.zip",
            strip_prefix = "rules_foreign_cc-8ccd83504b2221b670fc0b83d78fcee5642f4cb1",
            sha256 = "e5e8289b236bf57cfed2e76a20756ddff90fec7c8a4633e6a028e65899ecb6c7",
        )