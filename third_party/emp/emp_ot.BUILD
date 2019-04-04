filegroup(
    name = "all",
    srcs = glob(["**"]),
    visibility = ["//visibility:public"],
)

filegroup(
    name = "headers",
    srcs = glob(["emp-ot/**/*.h"]),
    visibility = ["//visibility:public"],
)

cc_library(
    name = "test_header",
    hdrs = ["test/test.h"],
    include_prefix = "emp-ot",
    visibility = ["//visibility:public"],
)
