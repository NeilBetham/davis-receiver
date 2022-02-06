filegroup(
  name = "mbedtls_srcs",
  srcs = glob([
    "library/*.c",
    "library/*.h",
  ]),
)

filegroup(
  name = "mbedtls_hdrs",
  srcs = glob([
      "include/mbedtls/*.h",
      "include/psa/*.h",
    ],
    exclude = [
      "include/mbedtls/mbedtls_config.h",
    ],
  ),
)

genrule(
  name = "mbedtls_config",
  srcs = ["@//deps/mbedtls:config/mbedtls_config.h"],
  outs = ["include/mbedtls/mbedtls_config.h"],
  cmd = "cp $(location @//deps/mbedtls:config/mbedtls_config.h) $(RULEDIR)/include/mbedtls/",
)

cc_library(
  name = "mbedtls-lib",
  srcs = [":mbedtls_srcs"],
  hdrs = [":mbedtls_hdrs", ":mbedtls_config"],
  linkstatic = True,
  strip_include_prefix = "include",
  copts = [
    "-ggdb",
    "-mthumb",
    "-mcpu=cortex-m4",
    "-mfpu=fpv4-sp-d16",
    "-mfloat-abi=softfp",
    "-specs=nano.specs",
    "-specs=nosys.specs",
    "-I external/mbedtls/library/",
  ],
  visibility = ["//visibility:public"],
)
