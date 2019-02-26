load("@rules_foreign_cc//tools/build_defs:configure.bzl", "configure_make")

licenses(["restricted"])

configure_make(
    name = "gmp",
    lib_source = "@org_gmplib//:all",
    configure_options = [
      "--enable-cxx",
      "CXX=g++",  #Needed presumably because of a bug in GMP's configure script.
    ],
    static_libraries = [
        "libgmp.a",
        "libgmpxx.a",
    ],
    visibility = ["//visibility:public"],
)
