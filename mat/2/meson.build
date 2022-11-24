project('task2', 'c',
  version: '1.0.0',
  default_options: [
    'buildtype=debug',
    'warning_level=3',
    'c_std=gnu2x',
    'b_sanitize=address',
  ],
  license: 'MIT'
)

cc = meson.get_compiler('c')
cc_args = []

if cc.get_id() == 'gcc'
  cc_args += [
    '-pipe',
    '-Wduplicated-branches',
    '-funsigned-char',
    '-Wimplicit-fallthrough',
    '-Wmisleading-indentation',
    '-Wunused',
    '-Wno-pedantic',
    '-Wno-format-zero-length',
    '-Wimplicit',
    '-Werror=format=2',
    '-Wno-format-nonliteral',
    '-Werror=init-self',
    '-Werror=missing-include-dirs',
    '-Werror=pointer-arith',
    '-Wstrict-prototypes',
    '-Werror=implicit-function-declaration',
  ]
elif cc.get_id() == 'clang'
  cc_args += [
    '-gcodeview',
    '-pipe',
    '-funsigned-char',
    '-Wimplicit-fallthrough',
    '-Wmisleading-indentation',
    '-Wunused',
    '-Wno-pedantic',
    '-Wno-format-zero-length',
    '-Wimplicit',
    '-Werror=format=2',
    '-Wno-format-nonliteral',
    '-Werror=init-self',
    '-Werror=missing-include-dirs',
    '-Werror=pointer-arith',
    '-Wno-strict-prototypes',
    '-Werror=implicit-function-declaration',
  ]
endif

add_project_arguments(cc.get_supported_arguments(cc_args), language: 'c')

subdir('src')
subdir('test')

main = executable('main', 'main.c',
  dependencies: libtask2_dep
)
