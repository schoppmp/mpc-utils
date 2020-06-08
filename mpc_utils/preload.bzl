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
            urls = ["https://github.com/abseil/abseil-cpp/archive/ccdd1d57b6386ebc26fb0c7d99b604672437c124.zip"],
            strip_prefix = "abseil-cpp-ccdd1d57b6386ebc26fb0c7d99b604672437c124",
            sha256 = "a463a791e1b5eaad461956495401357efb792fcdbf47b5737ec420bb54c804b6",
        )
    if "com_github_nelhage_rules_boost" not in native.existing_rules():
        http_archive(
            name = "com_github_nelhage_rules_boost",
            sha256 = "ea88159d5b91a852de0cd8ccdc78c9ca42c54538241aa7ff727de3544da7f051",
            strip_prefix = "rules_boost-fe9a0795e909f10f2bfb6bfa4a51e66641e36557",
            url = "https://github.com/nelhage/rules_boost/archive/fe9a0795e909f10f2bfb6bfa4a51e66641e36557.zip",
        )
    if "com_github_schoppmp_rules_oblivc" not in native.existing_rules():
        http_archive(
            name = "com_github_schoppmp_rules_oblivc",
            sha256 = "10f53f0fab3e374fdef83dc4279d902a357993a87c91d6de3c30e64db59f87ee",
	    url = "https://github.com/schoppmp/rules_oblivc/archive/d33ab3a707b9d6e9a79a683e660e572ab8e92f16.zip",
            strip_prefix = "rules_oblivc-d33ab3a707b9d6e9a79a683e660e572ab8e92f16",
        )
    if "rules_foreign_cc" not in native.existing_rules():
        http_archive(
            name = "rules_foreign_cc",
            url = "https://github.com/bazelbuild/rules_foreign_cc/archive/74b146dc87d37baa1919da1e8f7b8aafbd32acd9.zip",
            strip_prefix = "rules_foreign_cc-74b146dc87d37baa1919da1e8f7b8aafbd32acd9",
            sha256 = "2de65ab702ebd0094da3885aae2a6a370df5edb4c9d0186096de79dffb356dbc",
        )
