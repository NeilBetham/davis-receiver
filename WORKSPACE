workspace(name = "davis-receiver")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# External dep compilation tools
http_archive(
  name = "rules_foreign_cc",
  url = "https://github.com/bazelbuild/rules_foreign_cc/archive/34fa6a796d6933cc0529950fee313579cd0d8c51.tar.gz",
  sha256 = "ff5b7cdb650377b5baf358e38abf7d849395add601116393b16e722ffb33eb4b",
  strip_prefix="rules_foreign_cc-34fa6a796d6933cc0529950fee313579cd0d8c51",
)
load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")
rules_foreign_cc_dependencies()

# Bazel Toolchain for ARM embedded
http_archive(
  name = "bazel_arm_none",
  url = "https://github.com/NeilBetham/bazel-arm-none/archive/858d8e0c6b060c456d96fe067a6d1b622c429872.tar.gz",
  sha256 = "28b18c1ec140fba42c036e8c7ba94dcdf06e3a4428726bf5fa3b6436b457c553",
  strip_prefix = "bazel-arm-none-858d8e0c6b060c456d96fe067a6d1b622c429872",
)
load("@bazel_arm_none//:deps.bzl", "toolchain_deps")
toolchain_deps()

# LWIP for networking stack
http_archive(
  name = "lwip",
  url = "http://download.savannah.nongnu.org/releases/lwip/lwip-2.1.2.zip",
  sha256 = "5e0ae1887bef5b27e35f92636a3e52f3bc67f1944f136d6c79d18e5d972f76b0",
  strip_prefix = "lwip-2.1.2",
  build_file = "@//deps/lwip:lwip.BUILD",
)

# FMT for string formatting
http_archive(
  name = "fmt",
  url = "https://github.com/fmtlib/fmt/archive/refs/tags/8.0.1.tar.gz",
  sha256 = "b06ca3130158c625848f3fb7418f235155a4d389b2abc3a6245fb01cb0eb1e01",
  strip_prefix = "fmt-8.0.1",
  build_file = "@//deps/fmt:fmt.BUILD",
)

# Picohttpparser for http request parsing
http_archive(
  name = "picohttp",
  url = "https://github.com/h2o/picohttpparser/archive/657ede0fdf49b806e48c85f128cc55fe8905ed6b.tar.gz",
  sha256 = "869abb24ae5d84456e6a9da982d075ee65ec1328b5d8c5103ba4410c50bbc6a0",
  strip_prefix = "picohttpparser-657ede0fdf49b806e48c85f128cc55fe8905ed6b",
  build_file = "@//deps/picohttp:picohttp.BUILD",
)

# mbedtls for https
http_archive(
  name = "mbedtls",
  url = "https://github.com/ARMmbed/mbedtls/archive/6249603e7c23aa15327ae740d1b8f66c32d3204d.tar.gz",
  sha256 = "6c26a5db0bdc41b7d12d749c66268136b6eceb79d81de5a62abebeebf554e470",
  strip_prefix = "mbedtls-6249603e7c23aa15327ae740d1b8f66c32d3204d",
  build_file = "@//deps/mbedtls:mbedtls.BUILD",
)

