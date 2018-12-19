load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

licenses(["restricted"])

cmake_external(
   name = "eigen",
   # These options help CMake to find prebuilt OpenBLAS, which will be copied into
   # $EXT_BUILD_DEPS/openblas by the cmake_external script
   cache_entries = {
       "BLA_VENDOR": "Open",
       "BLAS_LIBRARIES": "$EXT_BUILD_DEPS/openblas/lib/libopenblas.a",
   },
   headers_only = True,
   lib_source = "@org_bitbucket_eigen//:all",
   deps = ["//third_party/openblas:openblas"],
   visibility = ["//visibility:public"],
)
