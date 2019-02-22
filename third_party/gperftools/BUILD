load("@rules_foreign_cc//tools/build_defs:configure.bzl", "configure_make")

licenses(["notice"])  # BSD 3-clause

configure_make(
    name = "gperftools",
    configure_options = [
        "--enable-shared=no",
        "--enable-frame-pointers",
        "--disable-libunwind",
    ],
    lib_source = "@com_github_gperftools_gperftools//:all",
    static_libraries = [
        "libtcmalloc_and_profiler.a",
    ],
    visibility = ["//visibility:public"],
)
