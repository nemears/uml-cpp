add_executable(umltest elementTest.cpp)
target_link_libraries(umltest PUBLIC gtest gmock gtest_main Uml)
add_test(umltest OverrideID_Test)