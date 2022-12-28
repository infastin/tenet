src_files = [
  'hungarian.cpp'
]

header_files = [
  'hungarian.hpp',
  'matrix.hpp'
]

liboctangle_dep = octangle.get_variable('liboctangle_dep')
libtask3 = both_libraries('task3', src_files,
  dependencies: liboctangle_dep
)

libtask3_dep = declare_dependency(
  sources: [src_files, header_files],
  link_with: libtask3,
  include_directories: include_directories('.'),
  dependencies: liboctangle_dep,
)
