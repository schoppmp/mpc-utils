load("@rules_foreign_cc//foreign_cc:configure.bzl", "configure_make")

licenses(["restricted"])

configure_make(
    name = "ntl",
    configure_options = [
        "GMP_PREFIX=\"$EXT_BUILD_DEPS/gmp/\"",
        "NTL_THREAD_BOOST=on",
    ],
    lib_source = "@net_shoup_ntl//:all",
    make_commands = [
        "cd src",
        "make",
        "make install",
    ],
    out_static_libs = ["libntl.a"],
    visibility = ["//visibility:public"],
    deps = [
        "//third_party/gmp",
    ],
)
