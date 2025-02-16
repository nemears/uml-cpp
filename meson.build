project('uml-cpp', 'cpp', version: '0.4.3')
include_dir = include_directories('include')
if (get_option('buildTests'))
  egm = dependency('egm')
  gtest = dependency('gtest', main : true, required: false)
  yaml_cpp = dependency('yaml-cpp')
  uml_tests = executable('uml_tests',
    'src/test/elementTest.cpp', 'src/test/commentTest.cpp', 'src/test/dependencyTest.cpp',
    include_directories: include_dir,
    dependencies: [gtest, yaml_cpp, egm],
    link_args : '-fsanitize=address',
    cpp_args : '-fsanitize=address'
  )
  test('uml_tests', uml_tests)
endif
