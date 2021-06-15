load("@bazel_arm_none//:utils.bzl", "gdb_bin")

cc_library(
  name = "davis-receiver-lib",
  hdrs = glob(["include/**/*.h"]),
  srcs = glob(["src/**/*.c"], exclude = ["src/startup.c"]),
  strip_include_prefix = "include/",
  copts = [
    "-mthumb",
    "-mcpu=cortex-m4",
    "-mfpu=fpv4-sp-d16",
    "-mfloat-abi=softfp",
    "-specs=nano.specs",
    "-specs=nosys.specs",
  ],
)

cc_binary(
  name = "davis-receiver",
  srcs = ["src/startup.c"],
  deps = [
    ":davis-receiver-lib",
    ":ld/TM4C129ENCZAD.ld",
  ],
  copts = [
    "-mthumb",
    "-mcpu=cortex-m4",
    "-mfpu=fpv4-sp-d16",
    "-mfloat-abi=softfp",
    "-specs=nano.specs",
    "-specs=nosys.specs",
  ],
  linkopts = [
    "-T $(location :ld/TM4C129ENCZAD.ld)",
    "-e Reset_Handler",
    "-mthumb",
    "-mcpu=cortex-m4",
    "-specs=nano.specs",
    "-specs=nosys.specs",
  ],
)

gdb_bin()

