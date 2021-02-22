pybind11_add_module(yuml_python yumlPythonBind.cpp)
target_link_libraries(yuml_python PUBLIC Uml Parsers yaml-cpp)