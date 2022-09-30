src_files = [
  'strutils.cpp',
]

header_files = [
  'stack_arr.hpp',
  'stack_list.hpp',
  'stack_utils.hpp',
  'strutils.hpp',
]

libtask1 = both_libraries('task1', src_files, install: false)

libtask1_dep = declare_dependency(
  sources: header_files,
  link_with: libtask1,
  include_directories: include_directories('.'),
)
