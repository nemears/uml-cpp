set(umlSources  element.cpp id.cpp set.cpp namedElement.cpp namespace.cpp package.cpp packageableElement.cpp
                templateableElement.cpp parameterableElement.cpp relationship.cpp directedRelationship.cpp packageMerge.cpp
                dependency.cpp classifier.cpp typedElement.cpp feature.cpp property.cpp structuredClassifier.cpp class.cpp 
                connectableElement.cpp encapsulatedClassifier.cpp multiplicityElement.cpp behavioralFeature.cpp behavior.cpp
                redefinableElement.cpp type.cpp structuralFeature.cpp generalization.cpp valueSpecification.cpp parameter.cpp
                literalSpecification.cpp literalString.cpp literalInt.cpp literalReal.cpp literalBool.cpp literalNull.cpp
                literalUnlimitedNatural.cpp dataType.cpp primitiveType.cpp instanceSpecification.cpp slot.cpp 
                operation.cpp opaqueBehavior.cpp behavioredClassifier.cpp association.cpp enumeration.cpp enumerationLiteral.cpp
                comment.cpp instanceValue.cpp expression.cpp abstraction.cpp realization.cpp usage.cpp model.cpp stereotype.cpp
                templateSignature.cpp templateParameter.cpp templateBinding.cpp templateParameterSubstitution.cpp profile.cpp
                profileApplication.cpp extension.cpp extensionEnd.cpp deployment.cpp deployedArtifact.cpp deploymentTarget.cpp
                artifact.cpp manifestation.cpp generalizationSet.cpp connector.cpp connectorEnd.cpp port.cpp
                interface.cpp interfaceRealization.cpp signal.cpp reception.cpp activity.cpp activityNode.cpp activityEdge.cpp
                objectNode.cpp pin.cpp action.cpp inputPin.cpp outputPin.cpp controlFlow.cpp objectFlow.cpp controlNode.cpp
                initialNode.cpp finalNode.cpp decisionNode.cpp joinNode.cpp forkNode.cpp mergeNode.cpp activityFinalNode.cpp
                flowFinalNode.cpp activityParameterNode.cpp centralBufferNode.cpp dataStoreNode.cpp executableNode.cpp
                exceptionHandler.cpp activityGroup.cpp activityPartition.cpp interruptibleActivityRegion.cpp constraint.cpp
                opaqueAction.cpp valuePin.cpp actionInputPin.cpp invocationAction.cpp callAction.cpp callBehaviorAction.cpp 
                elementImport.cpp packageImport.cpp redefinableTemplateSignature.cpp classifierTemplateParameter.cpp 
                parameterSet.cpp

                #parsers
                parsers/parser.cpp parsers/emitterMetaData.cpp

                #managers
                managers/abstractManager.cpp managers/managerNode.cpp

                #CMAKE option for set
                PARENT_SCOPE)