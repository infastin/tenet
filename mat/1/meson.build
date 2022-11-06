project('task1', 'cpp',
  version: '1.0.0',
  default_options: [
    'buildtype=debugoptimized',
    'warning_level=3',

  ],
  license: 'MIT'
)

cxx = meson.get_compiler('cpp')
cxx_args = ['-std=c++2b']

if cxx.get_id() == 'gcc'
  cxx_args += [
    '-pipe',
    '-Wduplicated-branches',
    '-funsigned-char',
    '-Wimplicit-fallthrough',
    '-Wmisleading-indentation',
    '-Wunused',
    '-Wno-pedantic',
    '-Wno-format-zero-length',
    '-Werror=format=2',
    '-Wno-format-nonliteral',
    '-Werror=init-self',
    '-Werror=missing-include-dirs',
    '-Werror=pointer-arith',
    ]
elif cxx.get_id() == 'clang'
  cxx_args += [
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
    ]
endif

add_project_arguments(cxx.get_supported_arguments(cxx_args), language: 'cpp')

subdir('src')

main = executable('main', 'main.cpp',
  dependencies: libtask1_dep
)
