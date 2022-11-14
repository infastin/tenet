src_files = [
]

header_files = [
]

libtask31 = both_libraries('task31', src_files)

libtask31_dep = declare_dependency(
  sources: [src_files, header_files],
  link_with: libtask31,
  include_directories: include_directories('.'),
)
