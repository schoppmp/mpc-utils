load("@rules_foreign_cc//foreign_cc:cmake.bzl", "cmake")

licenses(["restricted"])

cmake(
    name = "eigen",
    out_headers_only = True,
    lib_source = "@org_tuxfamily_eigen//:all",
    out_include_dir = "include/eigen3",
    visibility = ["//visibility:public"],
)
