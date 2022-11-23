src_files = [
  'adj_list.c',
  'disjoint_set.c',
  'edge.c',
  'algorithm.c',
]

header_files = [
  'adj_list.h',
  'disjoint_set.h',
  'edge.h',
  'algorithm.h',
]

libtribble_dep = tribble.get_variable('libtribble_dep')
libtask2 = both_libraries('task2', src_files,
  dependencies: libtribble_dep,
)

libtask2_dep = declare_dependency(
  sources: [src_files, header_files],
  link_with: libtask2,
  include_directories: include_directories('.'),
  dependencies: libtribble_dep,
)
