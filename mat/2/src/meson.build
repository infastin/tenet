src_files = [
]

header_files = [
]

fmt_dep = dependency('fmt')

libtask2 = both_libraries('task2',
  src_files,
  dependencies: [fmt_dep],
)

libtask2_dep = declare_dependency(
  sources: [src_files, header_files],
  link_with: libtask2,
  dependencies: [fmt_dep],
  include_directories: include_directories('.'),
)
