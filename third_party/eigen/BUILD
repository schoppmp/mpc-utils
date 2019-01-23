load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

licenses(["restricted"])

cmake_external(
    name = "eigen",
    headers_only = True,
    lib_source = "@org_bitbucket_eigen//:all",
    out_include_dir = "include/eigen3",
    visibility = ["//visibility:public"],
)
