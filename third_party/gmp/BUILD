load("@rules_foreign_cc//tools/build_defs:configure.bzl", "configure_make")

licenses(["restricted"])

configure_make(
    name = "gmp",
    lib_source = "@org_gmplib//:all",
    #configure_options = [
    #  "--enable-cxx",
    #],
    static_libraries = ["libgmp.a"],
    visibility = ["//visibility:public"],
)
