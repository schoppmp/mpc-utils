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
            urls = ["https://github.com/abseil/abseil-cpp/archive/3b4a16abad2c2ddc494371cc39a2946e36d35d11.zip"],
            strip_prefix = "abseil-cpp-3b4a16abad2c2ddc494371cc39a2946e36d35d11",
            sha256 = "64c43686598cf554d9e91fa9a6dafd87a84d7ce9f667dccdd3971b5b249960dc",
        )
    if "com_github_nelhage_rules_boost" not in native.existing_rules():
        http_archive(
            name = "com_github_nelhage_rules_boost",
            strip_prefix = "rules_boost-c13a880269cc044c4b5e90046625339836771d77",
            url = "https://github.com/nelhage/rules_boost/archive/c13a880269cc044c4b5e90046625339836771d77.zip",
            sha256 = "19a51f2f67e6bd2d1655a0641568bd88211db641a77c27fcdab7d9ad6de7dc00",
        )
    if "com_github_schoppmp_rules_oblivc" not in native.existing_rules():
        http_archive(
            name = "com_github_schoppmp_rules_oblivc",
	        url = "https://github.com/schoppmp/rules_oblivc/archive/84d26968342777bf9c6b9734d560fa1464ac865f.zip",
            strip_prefix = "rules_oblivc-84d26968342777bf9c6b9734d560fa1464ac865f",
            sha256 = "d82326ab62c4a383ce4284446739181236b6744fba874d5fe53bfbdde369da29",
        )
    if "rules_foreign_cc" not in native.existing_rules():
        http_archive(
            name = "rules_foreign_cc",
            url = "https://github.com/bazelbuild/rules_foreign_cc/archive/1f48d7756c8cb1361ce9cb9c7205036047d162e0.zip",
            strip_prefix = "rules_foreign_cc-1f48d7756c8cb1361ce9cb9c7205036047d162e0",
            sha256 = "382a2343cfd4cd1d94d7d89d9d2b7e15e48fc614e9371b4aeaf13e1161794b40",
        )
