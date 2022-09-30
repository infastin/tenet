src_files = [
  't2func.cpp',
]

header_files = [
  't2func.hpp',
  'graph.hpp',
]

libtask2 = both_libraries('task2', src_files)

libtask2_dep = declare_dependency(
  sources: header_files,
  link_with: libtask2,
  include_directories: include_directories('.'),
)
