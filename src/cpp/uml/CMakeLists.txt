add_library(Uml element.cpp id.cpp set.cpp umlManager.cpp namedElement.cpp namespace.cpp package.cpp packageableElement.cpp
                templateableElement.cpp parameterableElement.cpp relationship.cpp directedRelationship.cpp packageMerge.cpp
                dependency.cpp classifier.cpp typedElement.cpp feature.cpp property.cpp class.cpp structuredClassifier.cpp
                redefinableElement.cpp type.cpp structuralFeature.cpp generalization.cpp valueSpecification.cpp 
                literalSpecification.cpp literalString.cpp literalInt.cpp literalReal.cpp literalBool.cpp literalNull.cpp
                literalUnlimitedNatural.cpp dataType.cpp primitiveType.cpp instanceSpecification.cpp slot.cpp 
                behavioralFeature.cpp behavior.cpp connectableElement.cpp multiplicityElement.cpp parameter.cpp
                operation.cpp opaqueBehavior.cpp behavioredClassifier.cpp association.cpp enumeration.cpp enumerationLiteral.cpp
                comment.cpp instanceValue.cpp expression.cpp abstraction.cpp realization.cpp usage.cpp model.cpp 
                templateableElement.cpp templateSignature.cpp parameterableElement.cpp templateParameter.cpp
                templateBinding.cpp templateParameterSubstitution.cpp redefinableElement.cpp profile.cpp stereotype.cpp)
# add_library(Uml   
#                 activity.cpp activityNode.cpp activityEdge.cpp objectNode.cpp  pin.cpp
#                 action.cpp inputPin.cpp outputPin.cpp controlFlow.cpp objectFlow.cpp initialNode.cpp finalNode.cpp decisionNode.cpp
#                 joinNode.cpp forkNode.cpp mergeNode.cpp parameterNode.cpp callBehaviorAction.cpp  createObjectAction.cpp
#                 profileApplication.cpp extension.cpp extensionEnd.cpp dependency.cpp 
#                 deployment.cpp deployedArtifact.cpp deploymentTarget.cpp artifact.cpp 
#                 manifestation.cpp generalizationSet.cpp)
if (UML_CLIENT_SERVER)
    target_sources(Uml PRIVATE managers/umlClient.cpp managers/umlServer.cpp)
endif()
# add_subdirectory(parsers)
# if (UML_CPP_BUILD_CLANG)
#     add_subdirectory(cpp)
# endif()
# target_link_libraries(Uml PUBLIC yaml-cpp Parsers)
if (UML_CLIENT_SERVER)
    add_executable(uml-server managers/main.cpp)
    target_link_libraries(uml-server PUBLIC Uml)
    target_link_libraries(uml-server PRIVATE Threads::Threads)
endif()