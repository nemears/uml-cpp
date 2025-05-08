project('uml-cpp', 'cpp', version: '0.5.0', default_options: ['cpp_std=c++23'])
include_dir = include_directories('include')
if (get_option('buildTests'))
  egm = dependency('egm')
  gtest = dependency('gtest', main : true, required: false)
  yaml_cpp = dependency('yaml-cpp')
  uml_tests = executable('uml_tests',
    'src/test/elementTest.cpp', 'src/test/commentTest.cpp', 'src/test/dependencyTest.cpp', 'src/test/classifierTest.cpp', 'src/test/generalizationTest.cpp', 'src/test/dataTypeTest.cpp', 'src/test/namedElementTest.cpp', 'src/test/instanceSpecificationTest.cpp', 'src/test/associationTest.cpp', 'src/test/enumerationTest.cpp', 'src/test/signalTest.cpp', 'src/test/valueSpecificationTest.cpp',
    include_directories: include_dir,
    dependencies: [gtest, yaml_cpp, egm],
    link_args : '-fsanitize=address',
    cpp_args : '-fsanitize=address'
  )
  test('uml_tests', uml_tests)
endif
