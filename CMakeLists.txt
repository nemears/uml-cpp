cmake_minimum_required(VERSION 3.12.0)
project(uml-cpp VERSION 0.1.0)
include(CTest)
enable_testing()

option(UML_CLIENT_SERVER "allows for building of src/cpp/uml/managers for restful server client classes" ON)
option(UML_BUILD_TESTS "build tests for uml library" ON)
option(UML_DEBUG "builds with debug tools and without optimizations" OFF)

if (UML_DEBUG)
    add_definitions( -DUML_DEBUG )
endif()

set (CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Threads REQUIRED)

IF(UNIX)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    add_compile_options(-Wall -Wextra -pedantic)
    IF(NOT UML_DEBUG) 
        message(STATUS "Optimizations enabled!")
        add_compile_options(-O3)
    ELSE()
        message(STATUS "debug build")
        add_compile_options(-O0 -fstandalone-debug)
    ENDIF()
ENDIF()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang") 
    option(UML_ADDRESS_SANITIZER "link executables to google address sanitizer" OFF)
endif()

# Headers
include_directories(include)

# Google Test dependency
IF(WIN32)
    set(gtest_force_shared_crt ON)
ENDIF()
add_subdirectory(libs/googletest)

# yaml-cpp dependency
add_subdirectory(libs/yaml-cpp)
set(YAML_CPP_BUILD_TESTS OFF)

# src
if(UML_CLIENT_SERVER)
    list(APPEND umlSources managers/protocol/umlServer.cpp managers/protocol/abstractClient.cpp)
else() 
    add_definitions( -DNO_UML_CLIENT_SERVER)
endif()
add_library(uml STATIC 

    # core
    src/uml/id.cpp

    # managers
    src/uml/managers/abstractManager.cpp src/uml/managers/managerNode.cpp
    src/uml/managers/filePersistencePolicy.cpp
    ## serialization
    src/uml/managers/serialization/open_uml/openUmlSerializationPolicy.cpp
    src/uml/managers/serialization/open_uml/emit.cpp src/uml/managers/serialization/open_uml/parse.cpp

    # types
    src/uml/types/element.cpp  src/uml/types/namedElement.cpp src/uml/types/namespace.cpp src/uml/types/package.cpp 
    src/uml/types/packageableElement.cpp src/uml/types/templateableElement.cpp src/uml/types/parameterableElement.cpp 
    src/uml/types/relationship.cpp src/uml/types/directedRelationship.cpp src/uml/types/packageMerge.cpp
    src/uml/types/dependency.cpp src/uml/types/classifier.cpp src/uml/types/typedElement.cpp src/uml/types/feature.cpp 
    src/uml/types/property.cpp src/uml/types/structuredClassifier.cpp src/uml/types/class.cpp src/uml/types/connectableElement.cpp 
    src/uml/types/encapsulatedClassifier.cpp src/uml/types/multiplicityElement.cpp src/uml/types/behavioralFeature.cpp 
    src/uml/types/behavior.cpp src/uml/types/redefinableElement.cpp src/uml/types/type.cpp src/uml/types/structuralFeature.cpp 
    src/uml/types/generalization.cpp src/uml/types/valueSpecification.cpp src/uml/types/parameter.cpp 
    src/uml/types/literalSpecification.cpp src/uml/types/literalString.cpp src/uml/types/literalInt.cpp src/uml/types/literalReal.cpp 
    src/uml/types/literalBool.cpp src/uml/types/literalNull.cpp src/uml/types/literalUnlimitedNatural.cpp src/uml/types/dataType.cpp 
    src/uml/types/primitiveType.cpp src/uml/types/instanceSpecification.cpp src/uml/types/slot.cpp src/uml/types/operation.cpp 
    src/uml/types/opaqueBehavior.cpp src/uml/types/behavioredClassifier.cpp src/uml/types/association.cpp src/uml/types/enumeration.cpp 
    src/uml/types/enumerationLiteral.cpp src/uml/types/comment.cpp src/uml/types/instanceValue.cpp src/uml/types/expression.cpp 
    src/uml/types/abstraction.cpp src/uml/types/realization.cpp src/uml/types/usage.cpp src/uml/types/model.cpp 
    src/uml/types/stereotype.cpp src/uml/types/templateSignature.cpp src/uml/types/templateParameter.cpp 
    src/uml/types/templateBinding.cpp src/uml/types/templateParameterSubstitution.cpp src/uml/types/profile.cpp
    src/uml/types/profileApplication.cpp src/uml/types/extension.cpp src/uml/types/extensionEnd.cpp src/uml/types/deployment.cpp 
    src/uml/types/deployedArtifact.cpp src/uml/types/deploymentTarget.cpp src/uml/types/artifact.cpp src/uml/types/manifestation.cpp 
    src/uml/types/generalizationSet.cpp src/uml/types/connector.cpp src/uml/types/connectorEnd.cpp src/uml/types/port.cpp
    src/uml/types/interface.cpp src/uml/types/interfaceRealization.cpp src/uml/types/signal.cpp src/uml/types/reception.cpp 
    src/uml/types/activity.cpp src/uml/types/activityNode.cpp src/uml/types/activityEdge.cpp src/uml/types/objectNode.cpp 
    src/uml/types/pin.cpp src/uml/types/action.cpp src/uml/types/inputPin.cpp src/uml/types/outputPin.cpp src/uml/types/controlFlow.cpp
    src/uml/types/objectFlow.cpp src/uml/types/controlNode.cpp src/uml/types/initialNode.cpp src/uml/types/finalNode.cpp 
    src/uml/types/decisionNode.cpp src/uml/types/joinNode.cpp src/uml/types/forkNode.cpp src/uml/types/mergeNode.cpp 
    src/uml/types/activityFinalNode.cpp src/uml/types/flowFinalNode.cpp src/uml/types/activityParameterNode.cpp 
    src/uml/types/centralBufferNode.cpp src/uml/types/dataStoreNode.cpp src/uml/types/executableNode.cpp 
    src/uml/types/exceptionHandler.cpp src/uml/types/activityGroup.cpp src/uml/types/activityPartition.cpp 
    src/uml/types/interruptibleActivityRegion.cpp src/uml/types/constraint.cpp src/uml/types/opaqueAction.cpp 
    src/uml/types/valuePin.cpp src/uml/types/actionInputPin.cpp src/uml/types/invocationAction.cpp src/uml/types/callAction.cpp 
    src/uml/types/callBehaviorAction.cpp src/uml/types/elementImport.cpp src/uml/types/packageImport.cpp 
    src/uml/types/redefinableTemplateSignature.cpp src/uml/types/classifierTemplateParameter.cpp src/uml/types/parameterSet.cpp
    

)
target_include_directories(uml PUBLIC include)
target_link_libraries(uml PUBLIC yaml-cpp)
if (UML_CLIENT_SERVER)
    add_executable(uml-server src/cpp/uml/managers/protocol/main.cpp)
    target_link_libraries(uml-server PUBLIC uml)
    target_link_libraries(uml-server PRIVATE Threads::Threads)
    if (UML_DEBUG AND UML_ADDRESS_SANITIZER) 
        target_link_libraries(uml-server PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    endif()
    if(WIN32)
        target_link_libraries(uml-server PRIVATE wsock32 ws2_32)
        target_link_libraries(uml PRIVATE wsock32 ws2_32)
    endif()
endif()
if (UML_BUILD_TESTS)
    add_subdirectory(src/test)
    list(TRANSFORM umltestSources PREPEND "src/test/")
    if (UML_CLIENT_SERVER)
        list(APPEND umltestSources src/cpp/test/restfulTest.cpp)
    endif()
    add_executable(umltest ${umltestSources})
    target_link_libraries(umltest PUBLIC gtest gmock gtest_main uml)
    if (UML_DEBUG AND UML_ADDRESS_SANITIZER) 
        target_link_libraries(umltest PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    endif()
    add_test(umltest umltest)
endif()


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
