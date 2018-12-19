load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

licenses(["notice"])

cmake_external(
  name = "openblas",
  # Values to be passed as -Dkey=value on the CMake command line;
  # here are serving to provide some CMake script configuration options
  cache_entries = {
     "NOFORTRAN": "on",
     "BUILD_WITHOUT_LAPACK": "no",
     # Do not install to lib64 on 64 bit systems
     "CMAKE_INSTALL_LIBDIR": "$INSTALLDIR/lib",
  },
  lib_source = "@com_github_xianyi_openblas//:all",
  static_libraries = ["libopenblas.a"],
  visibility = ["//visibility:public"],
  make_commands = [
    "make -j5",
    "make install",
  ]
)
