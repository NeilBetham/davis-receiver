workspace(name = "davis-receiver")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# External dep compilation tools
http_archive(
  name = "rules_foreign_cc",
  url = "https://github.com/bazelbuild/rules_foreign_cc/archive/d54c78ab86b40770ee19f0949db9d74a831ab9f0.tar.gz",
  sha256 = "e7446144277c9578141821fc91c55a61df7ae01bda890902f7286f5fd2f6ae46",
  strip_prefix="rules_foreign_cc-d54c78ab86b40770ee19f0949db9d74a831ab9f0",
)
load("@rules_foreign_cc//:workspace_definitions.bzl", "rules_foreign_cc_dependencies")
rules_foreign_cc_dependencies()

# Bazel Toolchain for ARM embedded
http_archive(
  name = "bazel_arm_none",
  url = "https://github.com/NeilBetham/bazel-arm-none/archive/59ae6a1e64d49fb6ba10cab971a0fcc9dc5ffb46.tar.gz",
  sha256 = "eaf339bb45d0f702caf3499d7699789314a2473c59374b713ec2876ac43aeb9c",
  strip_prefix = "bazel-arm-none-59ae6a1e64d49fb6ba10cab971a0fcc9dc5ffb46",
)
load("@bazel_arm_none//:deps.bzl", "toolchain_deps")
toolchain_deps()
