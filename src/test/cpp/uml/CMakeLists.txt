add_executable(umltest elementTest.cpp)
target_link_libraries(umltest gtest)
add_test(umltest OverrideID_Test)