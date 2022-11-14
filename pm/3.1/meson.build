project('task31', 'cpp',
  version: '1.0.0',
  default_options: [
    'buildtype=debugoptimized',
    'warning_level=3',
    'cpp_std=gnu++20',
  ],
  license: 'MIT'
)

subdir('src')

executable('main', 'main.cpp',
  dependencies: libtask31_dep
)
