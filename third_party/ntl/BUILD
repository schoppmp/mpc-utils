load(":ntl.bzl", "ntl_build")

licenses(["restricted"])

ntl_build(
  name = "ntl",
  lib_source = "@net_shoup_ntl//:all",
  configure_command = "src/configure",
  deps = [
    "//third_party/gmp:gmp",
  ],
  static_libraries = ["libntl.a"],
  visibility = ["//visibility:public"],
)
