cc_library(
  name = "eigen",
  hdrs = [
    "eigen.hpp",
  ],
  deps = [
    "//third_party/eigen:eigen",
    "@boost//:serialization",
  ],
)

cc_test(
  name = "eigen_test",
  srcs = [
    "eigen_test.cpp",
  ],
  size = "small",
  deps = [
    ":eigen",
    "@googletest//:gtest_main",
  ]
)

cc_library(
  name = "ntl",
  hdrs = [
    "ntl.hpp",
  ],
  deps = [
    "//third_party/ntl:ntl",
    "@boost//:serialization",
  ],
)

cc_test(
  name = "ntl_test",
  srcs = [
    "ntl_test.cpp",
  ],
  size = "small",
  deps = [
    ":ntl",
    "@googletest//:gtest_main",
  ]
)
