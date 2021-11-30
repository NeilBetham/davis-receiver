filegroup(
  name = "lwip_sources",
  srcs = glob([
      "src/**/*.c",
      "src/**/*.h",
    ],
    exclude = [
      "src/include/**/*.h",
      "src/apps/http/**/*.c"
    ],
  ),
)

filegroup(
  name = "lwip_headers",
  srcs = glob(["src/include/**/*.h"])
)

genrule(
  name = "lwip_config",
  srcs = [
    "@//deps/lwip:config/lwipopts.h",
    "@//deps/lwip:config/cc.h",
  ],
  outs = [
    "src/include/lwipopts.h",
    "src/include/arch/cc.h",
  ],
  cmd = "\n".join([
    "cp $(location @//deps/lwip:config/lwipopts.h) $(RULEDIR)/src/include/",
    "cp $(location @//deps/lwip:config/cc.h) $(RULEDIR)/src/include/arch/cc.h",
  ])
)

cc_library(
  name = "lwip-lib",
  srcs = [":lwip_sources"],
  hdrs = [":lwip_headers", ":lwip_config"],
  deps = ["@//:sys-timing"],
  linkstatic = True,
  strip_include_prefix = "src/include",
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

