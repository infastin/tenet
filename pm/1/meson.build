project('task1', 'cpp',
  version: '1.0.0',
  default_options: [
    'buildtype=debug',
    'warning_level=3',
    'cpp_std=c++20'
  ],
  license: 'MIT'
)

subdir('src')

executable('main', 'main.cpp',
  dependencies: libtask1_dep,
)
