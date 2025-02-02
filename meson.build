project('uml-cpp', 'cpp', version: '0.4.3')
include_dir = include_directories('include')
egm = dependency('egm')
if (get_option('buildTests'))
  gtest = dependency('gtest', main : true, required: false)
  yaml_cpp = dependency('yaml-cpp')
  uml_tests = executable('uml_tests',
    'src/test/elementTest.cpp',
    include_directories: include_dir,
    dependencies: [gtest, yaml_cpp, egm]
  )
  test('uml_tests', uml_tests)
endif
