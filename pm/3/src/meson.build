src_files = [
]

header_files = [
]

libtask3 = both_libraries('task3', src_files)

libtask3_dep = declare_dependency(
  sources: header_files,
  link_with: libtask3,
  include_directories: include_directories('.'),
)
