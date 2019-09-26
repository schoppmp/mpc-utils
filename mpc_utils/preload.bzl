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
            url = "https://github.com/nelhage/rules_boost/archive/a1dd05e7e9178f8aad86e39f3a5b377902eae5b2.zip",
            sha256 = "f9afc8e2d3ef2cca277767745af02666d139dd285d0b820d0ce92238a457bac4",
            strip_prefix = "rules_boost-a1dd05e7e9178f8aad86e39f3a5b377902eae5b2",
        )
    if "com_github_schoppmp_rules_oblivc" not in native.existing_rules():
        http_archive(
            name = "com_github_schoppmp_rules_oblivc",
            sha256 = "86a4e546eecc06532ee2b0df19d3e8ab9a9ca32d65b7b3e4348da8c09ed4aa1a",
            url = "https://github.com/schoppmp/rules_oblivc/archive/dfc8c1fd3d6b5c6a4be12aa34f65d628230d2e87.zip",
            strip_prefix = "rules_oblivc-dfc8c1fd3d6b5c6a4be12aa34f65d628230d2e87",
        )
    if "rules_foreign_cc" not in native.existing_rules():
        http_archive(
            name = "rules_foreign_cc",
            url = "https://github.com/bazelbuild/rules_foreign_cc/archive/16ddc00bd4e1b3daf3faee1605a168f5283326fa.zip",
            strip_prefix = "rules_foreign_cc-16ddc00bd4e1b3daf3faee1605a168f5283326fa",
            sha256 = "54ef1b6a31f7cd0f1c707efb1dc670ab86d2d7238af845108f31ed9e6d0fdf01",
        )