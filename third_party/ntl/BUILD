load(":ntl.bzl", "ntl_build")

licenses(["restricted"])

ntl_build(
    name = "ntl",
    configure_command = "src/configure",
    lib_source = "@net_shoup_ntl//:all",
    static_libraries = ["libntl.a"],
    visibility = ["//visibility:public"],
    deps = [
        "//third_party/gmp",
    ],
)
