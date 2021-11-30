cc_library(
  name = "picohttp",
  srcs = ["picohttpparser.c"],
  hdrs = ["picohttpparser.h"],
  linkstatic = True,
  copts = [
    "-ggdb",
    "-mthumb",
    "-mcpu=cortex-m4",
    "-mfpu=fpv4-sp-d16",
    "-mfloat-abi=softfp",
    "-specs=nano.specs",
    "-specs=nosys.specs",
  ],
  visibility = ["//visibility:public"],
)
