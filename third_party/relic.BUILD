load("@rules_foreign_cc//foreign_cc:cmake.bzl", "cmake")

licenses(["notice"])  # Apache

filegroup(
    name = "all",
    srcs = glob(["**"]),
)

cmake(
    name = "relic",
    cache_entries = {
        # Needed for EMP.
        "ALIGN": "16",
        "ARCH": "X64",
        "ARITH": "curve2251-sse",
        "CHECK": "off",
        "FB_POLYN": "251",
        "FB_METHD": "INTEG;INTEG;QUICK;QUICK;QUICK;QUICK;LOWER;SLIDE;QUICK",
        "FB_PRECO": "on",
        "FB_SQRTF": "off",
        "EB_METHD": "PROJC;LODAH;COMBD;INTER",
        "EC_METHD": "CHAR2",
        "COMP": "-msse4.2 -mpclmul",
        "TIMER": "CYCLE",
        "WITH": "MD;DV;BN;FB;EB;EC",
        "WSIZE": "64",

        # For Bazel
        "SHLIB": "off",
        "STBIN": "on",
        "CMAKE_PREFIX_PATH": "$EXT_BUILD_DEPS/gmp/",
    },
    lib_source = ":all",
    out_static_libs = [
        "librelic_s.a",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "@mpc_utils//third_party/gmp",
    ],
)
