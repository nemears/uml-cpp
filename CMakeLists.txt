cmake_minimum_required(VERSION 3.12.0)
project(uml-cpp VERSION 0.4.0)
include(CTest)
enable_testing()

option(UML_BUILD_TESTS "build tests for uml library" ON)
option(UML_DEBUG "builds with debug tools and without optimizations" OFF)
option(NIX_BUILD "indicates that the paths to the libs are not in the libs folder, must set GTEST_PATH and YAML_CPP_PATH" OFF)
option(WEBSOCKET_BUILD "indicated that the websocket uml-kitchen functionality will be built" OFF)
set(GTEST_PATH "libs/googletest" CACHE STRING "location of googletest library")
set(YAML_CPP_PATH "libs/yaml-cpp" CACHE STRING "location of yaml-cpp library")
set(WEBSOCKETPP_PATH "" CACHE STRING "locatoin of websocketpp library")

message(STATUS "NIX_BUILD=${NIX_BUILD}")
message(STATUS "GTEST_PATH=${GTEST_PATH}")
message(STATUS "YAML_CPP_PATH=${YAML_CPP_PATH}")
message(STATUS "CMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}")

if (UML_DEBUG)
    add_definitions( -DUML_DEBUG )
endif()

set (CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

# options to generate compile_commands.json and get to work with clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
if(CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES 
		${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

find_package(Threads REQUIRED)

IF(UNIX)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    add_compile_options(-Wall -Wextra -pedantic)
    IF(NOT UML_DEBUG AND CMAKE_BUILD_TYPE STREQUAL RELEASE) 
        message(STATUS "Optimizations enabled!")
        add_compile_options(-O3)
    ELSE()
        message(STATUS "debug build")
        if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            add_compile_options(-fstandalone-debug)
        endif()
        # add_compile_options(-Og)
    ENDIF()
ENDIF()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang") 
    option(UML_ADDRESS_SANITIZER "link executables to google address sanitizer" OFF)
endif()

# Headers
include_directories(include)
IF(WIN32)
    set(gtest_force_shared_crt ON)
ENDIF()

find_package(PkgConfig)
if (UML_BUILD_TESTS)
    pkg_check_modules(GTEST REQUIRED gtest)
    pkg_check_modules(GTEST_MAIN REQUIRED gtest_main)
    include_directories(${YAML_CPP_PATH}/include ${GTEST_INCLUDE_DIRS} ${GTEST_MAIN_INCLUDE_DIRS})
    link_directories(${YAML_CPP_PATH}/lib ${GTEST_LIBRARY_DIRS} ${GTEST_MAIN_LIBRARY_DIRS})
else()
    include_directories(${YAML_CPP_PATH}/include)
    link_directories(${YAML_CPP_PATH}/lib)
endif()

if (WEBSOCKET_BUILD)
    find_package(PkgConfig)
    pkg_check_modules(ASIO REQUIRED asio)
    pkg_check_modules(OPENSSL REQUIRED openssl)
    find_package(WEBSOCKETPP CONFIG REQUIRED websocketpp)
    include_directories(${ASIO_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIRS} ${WEBSOCKETPP_INCLUDE_DIR})
    add_definitions( -DASIO_STANDALONE )
    if (${WEBSOCKETPP_FOUND})
        message(STATUS "found websocketpp!")
        message(STATUS "websocketpp path: ${WEBSOCKETPP_INCLUDE_DIR}")
    else()
        message(STATUS "did not find websocketpp!")
    endif()
    add_library(uml-kitchen-cpp STATIC 
        src/uml/managers/protocol/umlKitchenClient.cpp 
    )
endif()

add_library(uml SHARED 

    # managers
    #src/uml/managers/abstractManager.cpp src/uml/managers/managerNode.cpp
    #src/uml/managers/filePersistencePolicy.cpp
    ## serialization
    #src/uml/managers/serialization/uml-cafe/openUmlSerializationPolicy.cpp
    #src/uml/managers/serialization/uml-cafe/emit.cpp src/uml/managers/serialization/uml-cafe/parse.cpp

    # types
    src/uml/types/element.cpp  src/uml/types/namedElement.cpp src/uml/types/namespace.cpp src/uml/types/package.cpp 
    src/uml/types/packageableElement.cpp src/uml/types/templateableElement.cpp src/uml/types/parameterableElement.cpp 
    # src/uml/types/relationship.cpp src/uml/types/directedRelationship.cpp src/uml/types/packageMerge.cpp
    # src/uml/types/dependency.cpp src/uml/types/classifier.cpp src/uml/types/typedElement.cpp src/uml/types/feature.cpp 
    # src/uml/types/property.cpp src/uml/types/structuredClassifier.cpp src/uml/types/class.cpp src/uml/types/connectableElement.cpp 
    # src/uml/types/encapsulatedClassifier.cpp src/uml/types/multiplicityElement.cpp src/uml/types/behavioralFeature.cpp 
    # src/uml/types/behavior.cpp src/uml/types/redefinableElement.cpp src/uml/types/type.cpp src/uml/types/structuralFeature.cpp 
    # src/uml/types/generalization.cpp src/uml/types/valueSpecification.cpp src/uml/types/parameter.cpp 
    # src/uml/types/literalSpecification.cpp src/uml/types/literalString.cpp src/uml/types/literalInt.cpp src/uml/types/literalReal.cpp 
    # src/uml/types/literalBool.cpp src/uml/types/literalNull.cpp src/uml/types/literalUnlimitedNatural.cpp src/uml/types/dataType.cpp 
    # src/uml/types/primitiveType.cpp src/uml/types/instanceSpecification.cpp src/uml/types/slot.cpp src/uml/types/operation.cpp 
    # src/uml/types/opaqueBehavior.cpp src/uml/types/behavioredClassifier.cpp src/uml/types/association.cpp src/uml/types/enumeration.cpp 
    # src/uml/types/enumerationLiteral.cpp src/uml/types/comment.cpp src/uml/types/instanceValue.cpp src/uml/types/expression.cpp 
    # src/uml/types/abstraction.cpp src/uml/types/realization.cpp src/uml/types/usage.cpp src/uml/types/model.cpp 
    # src/uml/types/stereotype.cpp src/uml/types/templateSignature.cpp src/uml/types/templateParameter.cpp 
    # src/uml/types/templateBinding.cpp src/uml/types/templateParameterSubstitution.cpp src/uml/types/profile.cpp
    # src/uml/types/profileApplication.cpp src/uml/types/extension.cpp src/uml/types/extensionEnd.cpp src/uml/types/deployment.cpp 
    # src/uml/types/deployedArtifact.cpp src/uml/types/deploymentTarget.cpp src/uml/types/artifact.cpp src/uml/types/manifestation.cpp 
    # src/uml/types/generalizationSet.cpp src/uml/types/connector.cpp src/uml/types/connectorEnd.cpp src/uml/types/port.cpp
    # src/uml/types/interface.cpp src/uml/types/interfaceRealization.cpp src/uml/types/signal.cpp src/uml/types/reception.cpp 
    # src/uml/types/activity.cpp src/uml/types/activityNode.cpp src/uml/types/activityEdge.cpp src/uml/types/objectNode.cpp 
    # src/uml/types/pin.cpp src/uml/types/action.cpp src/uml/types/inputPin.cpp src/uml/types/outputPin.cpp src/uml/types/controlFlow.cpp
    # src/uml/types/objectFlow.cpp src/uml/types/controlNode.cpp src/uml/types/initialNode.cpp src/uml/types/finalNode.cpp 
    # src/uml/types/decisionNode.cpp src/uml/types/joinNode.cpp src/uml/types/forkNode.cpp src/uml/types/mergeNode.cpp 
    # src/uml/types/activityFinalNode.cpp src/uml/types/flowFinalNode.cpp src/uml/types/activityParameterNode.cpp 
    # src/uml/types/centralBufferNode.cpp src/uml/types/dataStoreNode.cpp src/uml/types/executableNode.cpp 
    # src/uml/types/exceptionHandler.cpp src/uml/types/activityGroup.cpp src/uml/types/activityPartition.cpp 
    # src/uml/types/interruptibleActivityRegion.cpp src/uml/types/constraint.cpp src/uml/types/opaqueAction.cpp 
    # src/uml/types/valuePin.cpp src/uml/types/actionInputPin.cpp src/uml/types/invocationAction.cpp src/uml/types/callAction.cpp 
    # src/uml/types/callBehaviorAction.cpp src/uml/types/elementImport.cpp src/uml/types/packageImport.cpp 
    # src/uml/types/redefinableTemplateSignature.cpp src/uml/types/classifierTemplateParameter.cpp src/uml/types/parameterSet.cpp
)
target_include_directories(uml PUBLIC include)
target_link_libraries(uml PUBLIC yaml-cpp)
if (WEBSOCKET_BUILD)
    target_include_directories(uml-kitchen-cpp PUBLIC include)
    target_link_libraries(uml-kitchen-cpp PUBLIC uml ${ASIO_LIBRARIES})
endif()
if (UML_BUILD_TESTS)
    add_subdirectory(src/test)
    list(TRANSFORM umltestSources PREPEND "src/test/")
    if (WEBSOCKET_BUILD)
        list(APPEND umltestSources src/test/umlKitchenTest.cpp)
    endif()
    add_executable(umltest ${umltestSources})
    if (NOT NIX_BUILD)
        if (NOT WEBSOCKET_BUILD)
            target_link_libraries(umltest PUBLIC gtest gmock gtest_main uml)
        else()
            target_link_libraries(umltest PUBLIC gtest gmock gtest_main ${ASIO_LIBRARIES} ${OPENSSL_LIBRARIES} uml uml-kitchen-cpp)
        endif()
    else()
        if (NOT WEBSOCKET_BUILD)
	        target_link_libraries(umltest PUBLIC ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} uml) 
        else()
            target_link_libraries(umltest PUBLIC ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} ${ASIO_LIBRARIES} ${OPENSSL_LIBRARIES} uml uml-kitchen-cpp)
        endif()
    endif()
    if (UML_DEBUG AND UML_ADDRESS_SANITIZER) 
        message("address sanitizer")
        target_link_libraries(umltest PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    endif()
    add_test(umltest umltest)
endif()

install(TARGETS uml)
install(DIRECTORY include/uml DESTINATION include)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
