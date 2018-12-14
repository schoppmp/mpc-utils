"""
Rule for building NTL from sources.
It is essentially a copy of @rules_foreign_cc//tools/build_defs:configure.bzl
and @rules_foreign_cc//tools/build_defs:configure_script.bzl, but adapted to
NTL's configure script.
"""

load(
    "@rules_foreign_cc//tools/build_defs:framework.bzl",
    "CC_EXTERNAL_RULE_ATTRIBUTES",
    "cc_external_rule_impl",
    "create_attrs",
)
load(
    "@rules_foreign_cc//tools/build_defs:detect_root.bzl",
    "detect_root",
)
load(
    "@rules_foreign_cc//tools/build_defs:cc_toolchain_util.bzl",
    "get_flags_info",
    "get_tools_info",
    "is_debug_mode",
)
load("@foreign_cc_platform_utils//:os_info.bzl", "OSInfo")
load(
  "@rules_foreign_cc//tools/build_defs:cc_toolchain_util.bzl",
  "absolutize_path_in_str"
)
load(
  "@rules_foreign_cc//tools/build_defs:framework.bzl",
  "ForeignCcDeps",
  "get_foreign_cc_dep"
)

def _ntl_build(ctx):
    copy_results = "copy_dir_contents_to_dir $BUILD_TMPDIR/$INSTALL_PREFIX $INSTALLDIR\n"

    attrs = create_attrs(
        ctx.attr,
        configure_name = "Configure",
        create_configure_script = _create_configure_script,
        postfix_script = copy_results,
    )
    return cc_external_rule_impl(ctx, attrs)

def _create_configure_script(configureParameters):
    ctx = configureParameters.ctx
    inputs = configureParameters.inputs

    root = detect_root(ctx.attr.lib_source)
    install_prefix = _get_install_prefix(ctx)

    tools = get_tools_info(ctx)
    flags = get_flags_info(ctx)

    define_install_prefix = "export INSTALL_PREFIX=\"" + _get_install_prefix(ctx) + "\"\n"

    configure = create_configure_script(
        ctx.workspace_name,
        ctx.attr._target_os[OSInfo],
        tools,
        flags,
        root,
        ctx.attr.configure_options,
        dict(ctx.attr.configure_env_vars),
        is_debug_mode(ctx),
        ctx.attr.configure_command,
        ctx.attr.deps,
        inputs,
    )
    return "\n".join([define_install_prefix, configure])

def _get_install_prefix(ctx):
    if ctx.attr.install_prefix:
        prefix = ctx.attr.install_prefix

        # If not in sandbox, or after the build, the value can be absolute.
        # So if the user passed the absolute value, do not touch it.
        if (prefix.startswith("/")):
            return prefix
        return prefix if prefix.startswith("./") else "./" + prefix
    if ctx.attr.lib_name:
        return "./" + ctx.attr.lib_name
    return "./" + ctx.attr.name

def _attrs():
    attrs = dict(CC_EXTERNAL_RULE_ATTRIBUTES)
    attrs.update({
        "configure_command": attr.string(default = "configure"),
        "configure_options": attr.string_list(),
        "configure_env_vars": attr.string_dict(),
        "install_prefix": attr.string(mandatory = False),
    })
    return attrs

ntl_build = rule(
    attrs = _attrs(),
    fragments = ["cpp"],
    output_to_genfiles = True,
    implementation = _ntl_build,
)



# The following is a copy of
# @rules_foreign_cc//tools/build_defs:configure_script.bzl, with the difference
# that the NTL configure script needs to be called differently. Hopefully, this
# will become obsolete in future versions of rules_foreign_cc.
def create_configure_script(
        workspace_name,
        target_os,
        tools,
        flags,
        root,
        user_options,
        user_vars,
        is_debug,
        configure_command,
        deps,
        inputs):
    vars = _get_configure_variables(tools, flags, user_vars)
    deps_flags = _define_deps_flags(deps, inputs)

    vars["LDFLAGS"] = vars["LDFLAGS"] + deps_flags["LDFLAGS"]
    vars["CPPFLAGS"] = deps_flags["CPPFLAGS"]

    env_vars_string = " ".join(["{}=\"{}\"".format(key, _join_flags_list(workspace_name, vars[key])) for key in vars])

    script = []
    for ext_dir in inputs.ext_build_dirs:
        script += ["increment_pkg_config_path $EXT_BUILD_ROOT/" + ext_dir.path]

    script += ["echo \"PKG_CONFIG_PATH=$PKG_CONFIG_PATH\""]

    # Some configure scripts assume they are called from the directory they are
    # in.
    script += ["CONFIGURE_SCRIPT=\"$EXT_BUILD_ROOT/{root}/{configure}\"".format(
        root = root,
        configure = configure_command
    )]
    script += ["cd $(dirname $CONFIGURE_SCRIPT)"]

    script += ["{env_vars} ./$(basename $CONFIGURE_SCRIPT) PREFIX=\"$BUILD_TMPDIR/$INSTALL_PREFIX\" CXXFLAGS=\"{cxxflags}\" {user_options}".format(
        env_vars = env_vars_string,
        cxxflags = _join_flags_list(workspace_name, vars["CXXFLAGS"]),
        user_options = " ".join(user_options),
    )]
    return "\n".join(script)

def _define_deps_flags(deps, inputs):
    # It is very important to keep the order for the linker => put them into list
    lib_dirs = []

    # Here go libraries built with Bazel
    gen_dirs_set = {}
    for lib in inputs.libs:
        dir_ = lib.dirname
        if not gen_dirs_set.get(dir_):
            gen_dirs_set[dir_] = 1
            lib_dirs += ["-L$EXT_BUILD_ROOT/" + dir_]

    include_dirs_set = {}
    for dir_list in inputs.include_dirs:
        for include_dir in dir_list:
            include_dirs_set[include_dir] = "-I$EXT_BUILD_ROOT/" + include_dir
    for header_list in inputs.headers:
        for header in header_list:
            include_dir = header.dirname
            if not include_dirs_set.get(include_dir):
                include_dirs_set[include_dir] = "-I$EXT_BUILD_ROOT/" + include_dir
    include_dirs = include_dirs_set.values()

    # For the external libraries, we need to refer to the places where
    # we copied the dependencies ($EXT_BUILD_DEPS/<lib_name>), because
    # we also want configure to find that same files with pkg-config
    # -config or other mechanics.
    # Since we need the names of include and lib directories under
    # the $EXT_BUILD_DEPS/<lib_name>, we ask the provider.
    gen_dirs_set = {}
    for dep in deps:
        external_deps = get_foreign_cc_dep(dep)
        if external_deps:
            for artifact in external_deps.artifacts:
                if not gen_dirs_set.get(artifact.gen_dir):
                    gen_dirs_set[artifact.gen_dir] = 1

                    dir_name = artifact.gen_dir.basename
                    include_dirs += ["-I$EXT_BUILD_DEPS/{}/{}".format(dir_name, artifact.include_dir_name)]
                    lib_dirs += ["-L$EXT_BUILD_DEPS/{}/{}".format(dir_name, artifact.lib_dir_name)]

    return {
        "LDFLAGS": lib_dirs,
        "CPPFLAGS": include_dirs,
    }

# See https://www.gnu.org/software/make/manual/html_node/Implicit-Variables.html
_CONFIGURE_FLAGS = {
    "CFLAGS": "cc",
    "CXXFLAGS": "cxx",
    "ARFLAGS": "cxx_linker_static",
    "ASFLAGS": "assemble",
    "LDFLAGS": "cxx_linker_executable",
    # missing: cxx_linker_shared
}

_CONFIGURE_TOOLS = {
    "CC": "cc",
    "CXX": "cxx",
    "AR": "cxx_linker_static",
    # missing: cxx_linker_executable
}

def _get_configure_variables(tools, flags, user_env_vars):
    vars = {}

    for flag in _CONFIGURE_FLAGS:
        flag_value = getattr(flags, _CONFIGURE_FLAGS[flag])
        if flag_value:
            vars[flag] = flag_value

    # Merge flags lists
    for user_var in user_env_vars:
        toolchain_val = vars.get(user_var)
        if toolchain_val:
            vars[user_var] = toolchain_val + [user_env_vars[user_var]]

    tools_dict = {}
    for tool in _CONFIGURE_TOOLS:
        tool_value = getattr(tools, _CONFIGURE_TOOLS[tool])
        if tool_value:
            tools_dict[tool] = [tool_value]

    # Replace tools paths if user passed other values
    for user_var in user_env_vars:
        toolchain_val = tools_dict.get(user_var)
        if toolchain_val:
            tools_dict[user_var] = [user_env_vars[user_var]]

    vars.update(tools_dict)

    # Put all other environment variables, passed by the user
    for user_var in user_env_vars:
        if not vars.get(user_var):
            vars[user_var] = [user_env_vars[user_var]]

    return vars

def _absolutize(workspace_name, text):
    return absolutize_path_in_str(workspace_name, "$EXT_BUILD_ROOT/", text)

def _join_flags_list(workspace_name, flags):
  return " ".join([_absolutize(workspace_name, flag) for flag in flags])
