project('task3', 'cpp',
  version: '1.0.0',
  default_options: [
    'buildtype=debugoptimized',
    'warning_level=3',
    'cpp_std=c++20'
  ],
  license: 'MIT'
)

subdir('src')

main = executable('main', 'main.cpp',
  dependencies: libtask3_dep
)
