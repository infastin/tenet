src_files = [
  'parser.cpp',
  'evaluator.cpp',
]

header_files = [
  'parser.hpp',
  'evaluator.hpp',
  'calculator.hpp',
]

libtask3 = both_libraries('task3', src_files)

libtask3_dep = declare_dependency(
  sources: header_files,
  link_with: libtask3,
  include_directories: include_directories('.'),
)
