package(default_visibility = ["//visibility:public"])

cc_library(
    name = "eigen",
    hdrs = [
        "eigen.hpp",
    ],
    deps = [
        "//third_party/eigen",
        "@boost//:serialization",
    ],
)

cc_test(
    name = "eigen_test",
    size = "small",
    srcs = [
        "eigen_test.cpp",
    ],
    deps = [
        ":eigen",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "ntl",
    hdrs = [
        "ntl.hpp",
    ],
    deps = [
        "//third_party/ntl",
        "@boost//:serialization",
    ],
)

cc_test(
    name = "ntl_test",
    size = "small",
    srcs = [
        "ntl_test.cpp",
    ],
    deps = [
        ":ntl",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "abseil",
    hdrs = [
        "abseil.hpp",
    ],
    deps = [
        "@com_google_absl//absl/numeric:int128",
        "@boost//:serialization",
    ],
)

cc_test(
    name = "abseil_test",
    size = "small",
    srcs = [
        "abseil_test.cpp",
    ],
    deps = [
        ":abseil",
        "@googletest//:gtest_main",
    ],
)