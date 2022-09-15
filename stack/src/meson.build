src_files = [
  'strutils.cpp',
]

header_files = [
  'stack_arr.hpp',
  'stack_list.hpp',
  'stack_utils.hpp',
  'strutils.hpp',
]

libstack = both_libraries('stack', src_files, install: false)

libstack_dep = declare_dependency(
  sources: header_files,
  link_with: libstack,
  include_directories: include_directories('.'),
)
