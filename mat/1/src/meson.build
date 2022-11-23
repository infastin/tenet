src_files = [
  'adj_list.c',
  'distance.c',
]

header_files = [
  'adj_list.h',
  'distance.h',
]

libtribble_dep = dependency('tribble-2.0')
libtask1 = both_libraries('task1', src_files,
  dependencies: libtribble_dep,
)

libtask1_dep = declare_dependency(
  sources: [src_files, header_files],
  link_with: libtask1,
  include_directories: include_directories('.'),
  dependencies: libtribble_dep,
)
