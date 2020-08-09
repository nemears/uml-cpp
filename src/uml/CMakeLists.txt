add_library(Uml namespace.cpp model.cpp element.cpp namedElement.cpp class.cpp)

#set(Boost_USE_STATIC_LIBS OFF) 
#set(Boost_USE_MULTITHREADED ON)  
#set(Boost_USE_STATIC_RUNTIME OFF) 
#set(BOOST_INCLUDEDIR C:/Boost/boost_1_73_0)
#set(BOOST_LIBRARYDIR C:/Boost/boost_1_73_0/lib)

#find_package(Boost 1.43.0 REQUIRED) 

#if(Boost_FOUND)
#    add_library(Uml namespace.cpp model.cpp element.cpp)
#    target_include_directories(Uml PRIVATE ${Boost_INCLUDE_DIRS})
#    target_link_libraries(Uml ${Boost_LIBRARIES})
#endif()