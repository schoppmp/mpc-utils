# Copyright 2017 The TensorFlow Authors. All rights reserved.
# Copyright 2016 The Bazel Authors. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# This version is based on tensorflow commit
# 89289dada9ba16617d125e4790c82e634b8d842b, and bazel commit
# 94e1dcc99373f24d042624d72800223c03e98825.


load("@bazel_tools//tools/build_defs/repo:utils.bzl", "workspace_and_buildfile")

"""Utilities for defining Bazel dependencies."""

def _is_windows(ctx):
    return ctx.os.name.lower().find("windows") != -1

def _wrap_bash_cmd(ctx, cmd):
    if _is_windows(ctx):
        bazel_sh = _get_env_var(ctx, "BAZEL_SH")
        if not bazel_sh:
            fail("BAZEL_SH environment variable is not set")
        cmd = [bazel_sh, "-l", "-c", " ".join(["\"%s\"" % s for s in cmd])]
    return cmd

def _get_env_var(ctx, name):
    if name in ctx.os.environ:
        return ctx.os.environ[name]
    else:
        return None

# Executes specified command with arguments and calls 'fail' if it exited with
# non-zero code
def _execute_and_check_ret_code(repo_ctx, cmd_and_args):
    result = repo_ctx.execute(cmd_and_args, timeout = 60)
    if result.return_code != 0:
        fail(("Non-zero return code({1}) when executing '{0}':\n" + "Stdout: {2}\n" +
              "Stderr: {3}").format(
            " ".join(cmd_and_args),
            result.return_code,
            result.stdout,
            result.stderr,
        ))

def _repos_are_siblings():
    return Label("@foo//bar").workspace_root.startswith("../")

# Apply a patch_file to the repository root directory
# Runs 'patch -p1'
def _apply_patch(ctx, patch_file):
    # Don't check patch on Windows, because patch is only available under bash.
    if not _is_windows(ctx) and not ctx.which("patch"):
        fail("patch command is not found, please install it")
    cmd = _wrap_bash_cmd(
        ctx,
        ["patch", "-p1", "-d", ctx.path("."), "-i", ctx.path(patch_file)],
    )
    _execute_and_check_ret_code(ctx, cmd)

def _apply_delete(ctx, paths):
    for path in paths:
        if path.startswith("/"):
            fail("refusing to rm -rf path starting with '/': " + path)
        if ".." in path:
            fail("refusing to rm -rf path containing '..': " + path)
    cmd = _wrap_bash_cmd(ctx, ["rm", "-rf"] + [ctx.path(path) for path in paths])
    _execute_and_check_ret_code(ctx, cmd)

def _third_party_http_archive(ctx):
    if not ctx.attr.url and not ctx.attr.urls:
        fail("At least one of url and urls must be provided")
    if ctx.attr.build_file and ctx.attr.build_file_content:
        fail("Only one of build_file and build_file_content can be provided.")

    all_urls = []
    if ctx.attr.urls:
        all_urls = ctx.attr.urls
    if ctx.attr.url:
        all_urls = [ctx.attr.url] + all_urls

    ctx.download_and_extract(
        all_urls,
        "",
        ctx.attr.sha256,
        ctx.attr.type,
        ctx.attr.strip_prefix,
    )
    if ctx.attr.delete:
        _apply_delete(ctx, ctx.attr.delete)
    if ctx.attr.patch_file != None:
        _apply_patch(ctx, ctx.attr.patch_file)

    for internal_src, external_dest in ctx.attr.link_files.items():
        ctx.symlink(Label(internal_src), ctx.path(external_dest))

    workspace_and_buildfile(ctx)

# Downloads and creates Bazel repos for dependencies.
#
# This is an upgrade for tf_http_archive that works with go/tfbr-thirdparty.
#
# For link_files, specify each dict entry as:
# "//path/to/source:file": "localfile"
third_party_http_archive = repository_rule(
    implementation = _third_party_http_archive,
    attrs = {
        "sha256": attr.string(mandatory = True),
        "url": attr.string(),
        "urls": attr.string_list(),
        "strip_prefix": attr.string(),
        "type": attr.string(),
        "delete": attr.string_list(),
        "build_file": attr.label(allow_single_file = True),
        "build_file_content": attr.string(),
        "patch_file": attr.label(),
        "link_files": attr.string_dict(),
        "workspace_file": attr.label(allow_single_file = True),
        "workspace_file_content": attr.string(),
    },
)
