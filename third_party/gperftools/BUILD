load("@rules_foreign_cc//tools/build_defs:configure.bzl", "configure_make")

licenses(["notice"])  # BSD 3-clause

configure_make(
    name = "gperftools",
    binaries = [
        "pprof",
    ],
    configure_options = [
        "--enable-shared=yes",
        "--enable-frame-pointers",
        "--disable-libunwind",
    ],
    lib_source = "@com_github_gperftools_gperftools//:all",
    shared_libraries = [
        "libtcmalloc_and_profiler.so.4",
    ],
    visibility = ["//visibility:public"],
)

filegroup(
    name = "pprof",
    srcs = [":gperftools"],
    output_group = "pprof",
    visibility = ["//visibility:public"],
)
