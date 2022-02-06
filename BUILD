load("@bazel_arm_none//:utils.bzl", "gdb_bin")

cc_library(
  name = "sys-timing",
  hdrs = ["include/sys_tick.h", "include/registers/core.h", "include/registers/register_helpers.h"],
  srcs = ["src/sys_tick.cpp"],
  strip_include_prefix = "include",
  copts = [
    "-ggdb",
    "-mthumb",
    "-mcpu=cortex-m4",
    "-mfpu=fpv4-sp-d16",
    "-mfloat-abi=softfp",
    "-specs=nano.specs",
    "-specs=nosys.specs",
    "-std=c++11",
    "-fno-exceptions",
  ],
  visibility = ["//visibility:public"],
)


cc_library(
  name = "davis-receiver-lib",
  hdrs = glob(["include/**/*.h"]),
  srcs = glob(["src/**/*.cpp"], exclude = ["src/startup.cpp", "src/handlers.cpp", "src/timing.cpp"]),
  strip_include_prefix = "include/",
  deps = [
    "@lwip//:lwip-lib",
    ":sys-timing",
    "@fmt//:fmt",
    "@picohttp//:picohttp",
    "@mbedtls//:mbedtls-lib",
  ],
  copts = [
    "-ggdb",
    "-mthumb",
    "-mcpu=cortex-m4",
    "-mfpu=fpv4-sp-d16",
    "-mfloat-abi=softfp",
    "-specs=nano.specs",
    "-specs=nosys.specs",
    "-std=c++11",
    "-fno-exceptions",
  ],
)

cc_binary(
  name = "davis-receiver",
  srcs = ["src/startup.cpp", "src/handlers.cpp"],
  deps = [
    ":davis-receiver-lib",
    ":ld/TM4C129ENCZAD.ld",
  ],
  copts = [
    "-ggdb",
    "-mthumb",
    "-mcpu=cortex-m4",
    "-mfpu=fpv4-sp-d16",
    "-mfloat-abi=softfp",
    "-specs=nano.specs",
    "-specs=nosys.specs",
    "-std=c++11",
    "-fno-exceptions",
  ],
  linkopts = [
    "-ggdb",
    "-T $(location :ld/TM4C129ENCZAD.ld)",
    "-mthumb",
    "-mcpu=cortex-m4",
    "-specs=nano.specs",
    "-specs=nosys.specs",
    "-lm",
    "-lsupc++",
    "-lstdc++",
    "-fno-exceptions",
  ],
)

gdb_bin()

