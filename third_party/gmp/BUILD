load("@rules_foreign_cc//foreign_cc:configure.bzl", "configure_make")

licenses(["restricted"])

configure_make(
    name = "gmp",
    lib_source = "@org_gmplib//:all",
    configure_options = [
      "--enable-cxx",
      "CXX=g++",  #Needed presumably because of a bug in GMP's configure script.
    ],
    out_static_libs = [
        "libgmp.a",
        "libgmpxx.a",
    ],
    visibility = ["//visibility:public"],
)
