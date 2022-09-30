project('task2', 'cpp',
  version: '1.0.0',
  default_options: [
    'buildtype=debugoptimized',
    'warning_level=3',
    'cpp_std=c++20'
  ],
  license: 'MIT'
)

task1_proj = subproject('task1')

subdir('src')

main = executable('main', 'main.cpp',
  dependencies: libtask2_dep
)
