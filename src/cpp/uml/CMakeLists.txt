add_library(Uml namespace.cpp model.cpp element.cpp namedElement.cpp class.cpp classifier.cpp property.cpp type.cpp typedElement.cpp 
                valueSpecification.cpp primitiveType.cpp literalString.cpp literalInt.cpp literalReal.cpp literalBool.cpp 
                instanceSpecification.cpp slot.cpp structuralFeature.cpp instanceValue.cpp operation.cpp behavior.cpp parameter.cpp
                opaqueBehavior.cpp activity.cpp activityNode.cpp activityEdge.cpp objectNode.cpp multiplicityElement.cpp pin.cpp
                action.cpp inputPin.cpp outputPin.cpp controlFlow.cpp objectFlow.cpp initialNode.cpp finalNode.cpp decisionNode.cpp
                joinNode.cpp forkNode.cpp mergeNode.cpp parameterNode.cpp callBehaviorAction.cpp expression.cpp createObjectAction.cpp
                relationship.cpp directedRelationship.cpp generalization.cpp packageableElement.cpp package.cpp literalSpecification.cpp
                feature.cpp behavioralFeature.cpp dataType.cpp redefinableElement.cpp structuredClassifier.cpp connectableElement.cpp)
add_subdirectory(parsers)