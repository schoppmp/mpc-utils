load("@rules_foreign_cc//tools/build_defs:configure.bzl", "configure_make")

licenses(["restricted"])

configure_make(
    name = "ntl",
    configure_options = [
        "GMP_PREFIX=\"$EXT_BUILD_DEPS/gmp/\"",
    ],
    lib_source = "@net_shoup_ntl//:all",
    make_commands = [
        "cd src",
        "make",
        "make install",
    ],
    static_libraries = ["libntl.a"],
    visibility = ["//visibility:public"],
    deps = [
        "//third_party/gmp",
    ],
)
