set(umlSources  element.cpp id.cpp set.cpp umlManager.cpp namedElement.cpp namespace.cpp package.cpp packageableElement.cpp
                templateableElement.cpp parameterableElement.cpp relationship.cpp directedRelationship.cpp packageMerge.cpp
                dependency.cpp classifier.cpp typedElement.cpp feature.cpp property.cpp class.cpp structuredClassifier.cpp
                redefinableElement.cpp type.cpp structuralFeature.cpp generalization.cpp valueSpecification.cpp 
                literalSpecification.cpp literalString.cpp literalInt.cpp literalReal.cpp literalBool.cpp literalNull.cpp
                literalUnlimitedNatural.cpp dataType.cpp primitiveType.cpp instanceSpecification.cpp slot.cpp 
                behavioralFeature.cpp behavior.cpp connectableElement.cpp multiplicityElement.cpp parameter.cpp
                operation.cpp opaqueBehavior.cpp behavioredClassifier.cpp association.cpp enumeration.cpp enumerationLiteral.cpp
                comment.cpp instanceValue.cpp expression.cpp abstraction.cpp realization.cpp usage.cpp model.cpp 
                templateableElement.cpp templateSignature.cpp parameterableElement.cpp templateParameter.cpp
                templateBinding.cpp templateParameterSubstitution.cpp redefinableElement.cpp profile.cpp stereotype.cpp
                profileApplication.cpp extension.cpp extensionEnd.cpp deployment.cpp deployedArtifact.cpp deploymentTarget.cpp
                artifact.cpp manifestation.cpp generalizationSet.cpp connector.cpp connectorEnd.cpp port.cpp
                encapsulatedClassifier.cpp interface.cpp interfaceRealization.cpp
                
                #parsers
                parsers/parser.cpp parsers/emitterMetaData.cpp

                #CMAKE option for set
                PARENT_SCOPE)
# add_library(Uml   
#                 activity.cpp activityNode.cpp activityEdge.cpp objectNode.cpp  pin.cpp
#                 action.cpp inputPin.cpp outputPin.cpp controlFlow.cpp objectFlow.cpp initialNode.cpp finalNode.cpp decisionNode.cpp
#                 joinNode.cpp forkNode.cpp mergeNode.cpp parameterNode.cpp callBehaviorAction.cpp  createObjectAction.cpp)
if (UML_CLIENT_SERVER)
    
endif()