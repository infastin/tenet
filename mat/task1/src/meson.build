src_files = [
  'path.cpp',
]

header_files = [
  'square_matrix.hpp',
  'path.hpp',
]

fmt_dep = dependency('fmt')

libtask1 = both_libraries('task1',
  src_files,
  dependencies: [fmt_dep],
)

libtask1_dep = declare_dependency(
  sources: [src_files, header_files],
  link_with: libtask1,
  dependencies: [fmt_dep],
  include_directories: include_directories('.'),
)
