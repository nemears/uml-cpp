#include "uml/managers/serialization/uml-cafe/emit.h"
#include "uml/types/stereotype.h"
#include <yaml-cpp/emitter.h>

using namespace std;

namespace UML {

template <class T, class V = T>
void emitFeatures(YAML::Emitter& emitter, T& el, EmitterData& data, void (*emitterFunc)(YAML::Emitter&, V&, EmitterData&)) {
    emitterFunc(emitter, el, data);
}

template <class T, class V = T, class ... Funcs>
void emitFeatures(YAML::Emitter& emitter, T& el, EmitterData& data, void (*emitterFunc)(YAML::Emitter&, V&, EmitterData&), Funcs... funcs) {
    emitFeatures(emitter, el, data, emitterFunc);
    emitFeatures(emitter, el, data, funcs...);
}

template <class T, class ... Funcs>
void emitElementTypeAndData(YAML::Emitter& emitter, T& el, EmitterData& data, string key, Funcs... funcs) {
    emitter << key << YAML::BeginMap;
    emitFeatures(emitter, el, data, funcs...);
    emitter << YAML::EndMap;
}

template <class T, class V = T>
void emitScope(YAML::Emitter& emitter, T& el, EmitterData& data, bool (*emitterFunc)(YAML::Emitter&, V&, EmitterData&)) {
    if (data.mode == SerializationMode::WHOLE) {
        return;
    }
    emitterFunc(emitter, el, data);
}

template <class T, class V = T, class ... Funcs>
void emitScope(YAML::Emitter& emitter, T& el, EmitterData& data, bool (*emitterFunc)(YAML::Emitter&, V&, EmitterData&), Funcs... funcs) {
    if (data.mode == SerializationMode::WHOLE) {
        return;
    }
    if (!emitterFunc(emitter, el, data)) {
        emitScope(emitter, el, data, funcs...);
    }
}

template <class T, class U, class S>
void emitSet(YAML::Emitter& emitter, U& el, string key, S& (U::*signature)()) {
    if (!(el.*signature)().empty()) {
        emitter << key << YAML::Value << YAML::BeginSeq;
        for (ID id : (el.*signature)().ids()) {
            emitter << id.string();
        }
        emitter << YAML::EndSeq;
    }
}

template <class T, class U, class S>
void emitCompositeSet(YAML::Emitter& emitter, U& el, EmitterData& data, string key, S& (U::*signature)()) {
    if (!(el.*signature)().empty()) {
        emitter << key << YAML::Value << YAML::BeginSeq;
        if (data.mode == SerializationMode::WHOLE) {
            for (auto& child : (el.*signature)()) {
                emitter << YAML::BeginMap;
                emitElementData(emitter, child, data);
                emitter << YAML::EndMap;
            }
        } else {
            for (ID id : (el.*signature)().ids()) {
                emitter << id.string();
            }
        }
        emitter << YAML::EndSeq;
    }
}

template <class T, class U, class S>
bool subsetContains(U& el, T& addedEl, S& (U::*signature)()) {
    return (el.*signature)().contains(addedEl.getID());
}

template <class T, class U>
bool subsetContains(U& el, T& addedEl, UmlPtr<T> (U::*signature)() const) {
    return (el.*signature)().id() == addedEl.getID();
}

template <class T, class U, class S, class ... Subsets>
bool subsetContains(U& el, T& addedEl, S& (U::*signature)(), Subsets ... subsets) {
    if (!(el.*signature)().contains(addedEl)) {
        return subsetContains(el, addedEl, subsets...);
    }
    return true;
}

template <class T, class U, class ... Subsets>
bool subsetContains(U& el, T& addedEl, UmlPtr<T> (U::*signature)(), Subsets ... subsets) {
    if ((el.*signature)().id() == addedEl.getID()) {
        return subsetContains(el, addedEl, subsets...);
    }
    return true;
}

template <class T>
bool subsetContainsID(T& el, ID addedID) {
    return false;
}

template <class T, class U>
bool subsetContainsID(U& el, ID addedID, UmlPtr<T> (U::*signature)() const) {
    return (el.*signature)().id() == addedID;
}

template <class T, class U, class S>
bool subsetContainsID(U& el, ID addedID, S& (U::*signature)()) {
    return (el.*signature)().contains(addedID);
}

template <class T, class U, class S, class ... Subsets>
bool subsetContainsID(U& el, ID addedID, S& (U::*signature)(), Subsets ... subsets) {
    if (!(el.*signature)().contains(addedID)) {
        return subsetContainsID(el, addedID, subsets...);
    }
    return true;
}

template <class T, class U, class ... Subsets>
bool subsetContainsID(U& el, ID addedID, UmlPtr<T> (U::*signature)() const, Subsets ... subsets) {
    if ((el.*signature)().id() != addedID) {
        return subsetContainsID(el, addedID, subsets...);
    }
    return true;
}

template <class T, class U, class S, class ... SubSets>
void emitCompositeSet(YAML::Emitter& emitter, U& el, EmitterData& data, string key, S& (U::*signature)(), SubSets ... subSets) {
    if (!(el.*signature)().empty()) {
        bool elementsAllInSubsets = true;
        for (ID id : (el.*signature)().ids()) {
            if (!subsetContainsID<T>(el, id, subSets...)) {
                elementsAllInSubsets = false;
            }
        }
        if (elementsAllInSubsets) {
            return;
        }
        emitter << key << YAML::Value << YAML::BeginSeq;
        if (data.mode == SerializationMode::WHOLE) {
            for (auto& child : (el.*signature)()) {
                if (subsetContains(el, child, subSets...)) {
                    continue;
                }
                emitter << YAML::BeginMap;
                emitElementData(emitter, child, data);
                emitter << YAML::EndMap;
            }
        } else {
            for (ID id : (el.*signature)().ids()) {
                if (subsetContainsID<T>(el, id, subSets...)) {
                    continue;
                }
                emitter << id.string();
            }
        }
        emitter << YAML::EndSeq;
    }
}

template <class T, class U, class S, class ... Subsets>
void emitSet(YAML::Emitter& emitter, U& el, string key, S& (U::*signature)(), Subsets ... subsets) {
    if (!(el.*signature)().empty()) {
        bool elementsAllInSubsets = true;
        for (ID id : (el.*signature)().ids()) {
            if (!subsetContainsID<T>(el, id, subsets...)) {
                elementsAllInSubsets = false;
            }
        }
        if (elementsAllInSubsets) {
            return;
        }
        emitter << key << YAML::Value << YAML::BeginSeq;
        for (ID id : (el.*signature)().ids()) {
            if (subsetContainsID<T>(el, id, subsets...)) {
                continue;
            }
            emitter << id.string();
        }
        emitter << YAML::EndSeq;
    }
}

template <class T, class U>
bool emitSingleton(YAML::Emitter& emitter, U& el, string key, UmlPtr<T> (U::*acessor)() const) {
    if ((el.*acessor)()) {
        emitter << key << YAML::Value << (el.*acessor)().id().string();
        return true;
    }
    return false;
}

template <class T, class U>
bool emitOwnedSingleton(YAML::Emitter& emitter, U& el, EmitterData& data, string key, UmlPtr<T> (U::*acessor)() const) {
    if ((el.*acessor)()) {
        if (data.mode == SerializationMode::WHOLE) {
            emitter << key << YAML::Value << YAML::BeginMap;
            emitElementData(emitter, *(el.*acessor)(), data); 
            emitter << YAML::EndMap;
        } else {
            emitter << key << YAML::Value << (el.*acessor)().id().string();
        }
        return true;
    }
    return false;
}

void emitElementData(YAML::Emitter& emitter, Element& el, EmitterData& data) {
    switch (el.getElementType()) {
        case ElementType::ABSTRACTION : {
            Abstraction& abstraction = el.as<Abstraction>();
            emitScope(emitter, abstraction, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, abstraction, data, "Abstraction",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitParameterableElementFeatures,
                        emitDependencyFeatures);
                        // TODO emitAbstractionFeatures;
            break;
        }
        case ElementType::ACTION_INPUT_PIN : {
            ActionInputPin& actionInputPin = el.as<ActionInputPin>();
            emitScope(emitter, actionInputPin, data,
                        emitActivityNodeScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, actionInputPin, data, "ActionInputPin", 
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitActivityNodeFeatures,
                        emitTypedElementFeatures,
                        emitObjectNodeFeatures,
                        emitMultiplicityElementFeatures,
                        emitPinFeatures,
                        emitActionInputPinFeatures);
            break;
        }
        case ElementType::ACTIVITY : {
            Activity& activity = el.as<Activity>();
            emitScope(emitter, activity, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, activity, data, "Activity",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitNamespaceFeatures,
                        emitParameterableElementFeatures,
                        emitTemplateableElementFeatures,
                        emitClassifierFeatures,
                        emitStructuredClassifierFeatures,
                        emitBehavioredClassifierFeatures,
                        emitClassFeatures,
                        emitBehaviorFeatures,
                        emitActivityFeatures);
            break;
        }
        case ElementType::ACTIVITY_FINAL_NODE : {
            ActivityFinalNode& activityFinalNode = el.as<ActivityFinalNode>();
            emitScope(emitter, activityFinalNode, data,
                        emitActivityNodeScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, activityFinalNode, data, "ActivityFinalNode", 
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitActivityNodeFeatures);
            break;
        }
        case ElementType::ACTIVITY_PARAMETER_NODE : {
            ActivityParameterNode& activityParameterNode = el.as<ActivityParameterNode>();
            emitScope(emitter, activityParameterNode, data,
                        emitActivityNodeScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, activityParameterNode, data, "ActivityParameterNode",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitActivityNodeFeatures,
                        emitTypedElementFeatures,
                        emitObjectNodeFeatures,
                        emitActivityParameterNodeFeatures);
            break;
        }
        case ElementType::ACTIVITY_PARTITION : {
            ActivityPartition& activityPartition = el.as<ActivityPartition>();
            emitScope(emitter, activityPartition, data,
                        emitActivityPartitionScope,
                        emitActivityGroupScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, activityPartition, data, "ActivityPartition",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitActivityPartitionFeatures);
            break;
        }
        case ElementType::ARTIFACT : {
            Artifact& artifact = el.as<Artifact>();
            emitScope(emitter, artifact, data, 
                        emitPackageableElementScope, 
                        emitParameterableElementScope,
                        emitNamedElementScope, 
                        emitElementScope);
            emitElementTypeAndData(emitter, artifact, data, "Artifact",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitNamespaceFeatures,
                        emitTemplateableElementFeatures,
                        emitParameterableElementFeatures,
                        emitClassifierFeatures,
                        emitArtifactFeatures);
            break;
        }
        case ElementType::ASSOCIATION : {
            Association& association = el.as<Association>();
            emitScope(emitter, association, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, association, data, "Association",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitNamespaceFeatures,
                        emitTemplateableElementFeatures,
                        emitParameterableElementFeatures,
                        emitClassifierFeatures,
                        emitAssociationFeatures);
            break;
        }
        case ElementType::CALL_BEHAVIOR_ACTION : {
            CallBehaviorAction& callBehaviorAction = el.as<CallBehaviorAction>();
            emitScope(emitter, callBehaviorAction, data,
                        emitActivityNodeScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, callBehaviorAction, data, "CallBehaviorAction",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitActivityNodeFeatures,
                        emitExecutableNodeFeatures,
                        emitActionFeatures,
                        emitInvocationActionFeatures,
                        emitCallActionFeatures,
                        emitCallBehaviorActionFeatures);
            break;
        }
        case ElementType::CENTRAL_BUFFER_NODE : {
            CentralBufferNode& centralBufferNode = el.as<CentralBufferNode>();
            emitScope(emitter, centralBufferNode, data,
                        emitActivityNodeScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, centralBufferNode, data, "CentralBufferNode",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitActivityNodeFeatures,
                        emitTypedElementFeatures,
                        emitObjectNodeFeatures);
            break;
        }
        case ElementType::CLASS : {
            Class& clazz = el.as<Class>();
            emitScope(emitter, clazz, data, 
                        emitPackageableElementScope, 
                        emitParameterableElementScope,
                        emitNamedElementScope, 
                        emitElementScope);
            emitElementTypeAndData(emitter, clazz, data, "Class",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitNamespaceFeatures,
                        emitTemplateableElementFeatures,
                        emitParameterableElementFeatures,
                        emitClassifierFeatures,
                        emitStructuredClassifierFeatures,
                        emitBehavioredClassifierFeatures,
                        emitClassFeatures);
            break;
        }
        case ElementType::CLASSIFIER_TEMPLATE_PARAMETER : {
            ClassifierTemplateParameter& classifierTemplateParameter = el.as<ClassifierTemplateParameter>();
            emitScope(emitter, classifierTemplateParameter, data,
                        emitTemplateParameterScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, classifierTemplateParameter, data, "ClassifierTemplateParameter", 
                        emitElementFeatures,
                        emitTemplateParameterFeatures,
                        emitClassifierTemplateParameter);
            break;
        }
        case ElementType::COMMENT : {
            Comment& comment = el.as<Comment>();
            emitScope(emitter, comment, data, emitElementScope);
            emitElementTypeAndData(emitter, comment, data, "Comment", emitElementFeatures, emitCommentFeatures);
            break;
        }
        case ElementType::CONNECTOR : {
            Connector& connector = el.as<Connector>();
            emitScope(emitter, connector, data,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, connector, data, "Connector",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitFeatureFeatures,
                        emitConnectorFeatures);
            break;
        }
        case ElementType::CONNECTOR_END : {
            ConnectorEnd& connectorEnd = el.as<ConnectorEnd>();
            emitScope(emitter, connectorEnd, data,
                        emitElementScope);
            emitElementTypeAndData(emitter, connectorEnd, data, "ConnectorEnd",
                        emitElementFeatures,
                        emitMultiplicityElementFeatures,
                        emitConnectorEndFeatures);
                        break;
        }
        case ElementType::CONSTRAINT : {
            Constraint& constraint = el.as<Constraint>();
            emitScope(emitter, constraint, data, 
                        emitConstraintScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, constraint, data, "Constraint",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitConstraintFeatures);
            break;
        }
        case ElementType::CONTROL_FLOW : {
            ControlFlow& controlFlow = el.as<ControlFlow>();
            emitScope(emitter, controlFlow, data,
                        emitActivityEdgeScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, controlFlow, data, "ControlFlow",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitActivityEdgeFeatures);
            break;
        }
        case ElementType::DATA_STORE_NODE : {
            DataStoreNode& dataStoreNode = el.as<DataStoreNode>();
            emitScope(emitter, dataStoreNode, data,
                        emitActivityNodeScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, dataStoreNode, data, "DataStoreNode",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitActivityNodeFeatures,
                        emitTypedElementFeatures,
                        emitObjectNodeFeatures);
            break;
        }
        case ElementType::DATA_TYPE : {
            DataType& dataType = el.as<DataType>();
            emitScope(emitter, dataType, data, 
                        emitPackageableElementScope, 
                        emitParameterableElementScope,
                        emitNamedElementScope, 
                        emitElementScope);
            emitElementTypeAndData(emitter, dataType, data, "DataType",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitNamespaceFeatures,
                        emitTemplateableElementFeatures,
                        emitParameterableElementFeatures,
                        emitClassifierFeatures,
                        emitDataTypeFeatures);
            break;
        }
        case ElementType::DECISION_NODE : {
            DecisionNode& decisionNode = el.as<DecisionNode>();
            emitScope(emitter, decisionNode, data,
                        emitActivityNodeScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, decisionNode, data, "DecisionNode", 
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitActivityNodeFeatures,
                        emitDecisionNodeFeatures);
            break;
        }
        case ElementType::DEPENDENCY : {
            Dependency& dependency = el.as<Dependency>();
            emitScope(emitter, dependency, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, dependency, data, "Dependency",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitParameterableElementFeatures,
                        emitDependencyFeatures);
            break;
        }
        case ElementType::DEPLOYMENT : {
            Deployment& deployment = el.as<Deployment>();
            emitScope(emitter, deployment, data,
                        emitDeploymentScope,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, deployment, data, "Deployment",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitParameterableElementFeatures,
                        emitDeploymentFeatures);
            break;
        }
        case ElementType::ELEMENT_IMPORT : {
            ElementImport& elementImport = el.as<ElementImport>();
            emitScope(emitter, elementImport, data,
                        emitElementImportScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, elementImport, data, "ElementImport",
                        emitElementFeatures,
                        emitElementImportFeatures);
            break;
        }
        case ElementType::ENUMERATION : {
            Enumeration& enumeration = el.as<Enumeration>();
            emitScope(emitter, enumeration, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, enumeration, data, "Enumeration", 
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitNamespaceFeatures,
                        emitTemplateableElementFeatures,
                        emitParameterableElementFeatures,
                        emitClassifierFeatures,
                        emitDataTypeFeatures,
                        emitEnumerationFeatures);
            break;
        }
        case ElementType::ENUMERATION_LITERAL : {
            EnumerationLiteral& literal = el.as<EnumerationLiteral>();
            emitScope(emitter, literal, data,
                        emitEnumerationLiteralScope, 
                        emitPackageableElementScope, 
                        emitParameterableElementScope,
                        emitNamedElementScope, 
                        emitElementScope);
            emitElementTypeAndData(emitter, literal, data, "EnumerationLiteral", 
                        emitElementFeatures, 
                        emitNamedElementFeatures, 
                        emitParameterableElementFeatures,
                        emitInstanceSpecificationFeatures);
            break;
        }
        case ElementType::EXCEPTION_HANDLER : {
            ExceptionHandler& exceptionHandler = el.as<ExceptionHandler>();
            emitScope(emitter, exceptionHandler, data,
                        emitExceptionHandlerScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, exceptionHandler, data, "ExceptionHandler", 
                        emitElementFeatures,
                        emitExceptionHandlerFeatures);
            break;
        }
        case ElementType::EXPRESSION : {
            Expression& expression = el.as<Expression>();
            emitScope(emitter, expression, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, expression, data, "Expression",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitTypedElementFeatures,
                        emitParameterableElementFeatures,
                        emitExpressionFeatures);
            break;
        }
        case ElementType::EXTENSION : {
            Extension& extension = el.as<Extension>();
            emitScope(emitter, extension, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, extension, data, "Extension", 
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitNamespaceFeatures,
                        emitParameterableElementFeatures,
                        emitTemplateableElementFeatures,
                        emitClassifierFeatures,
                        emitExtensionFeatures);
            break;
        }
        case ElementType::EXTENSION_END : {
            ExtensionEnd& extensionEnd = el.as<ExtensionEnd>();
            emitScope(emitter, extensionEnd, data,
                        emitPropertyScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, extensionEnd, data, "ExtensionEnd",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitTypedElementFeatures,
                        emitFeatureFeatures,
                        emitStructuralFeatureFeatures,
                        emitPropertyFeatures);
            break;
        }
        case ElementType::FINAL_NODE : {
            FinalNode& finalNode = el.as<FinalNode>();
            emitScope(emitter, finalNode, data,
                        emitActivityNodeScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, finalNode, data, "FinalNode", 
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitActivityNodeFeatures);
            break;
        }
        case ElementType::FLOW_FINAL_NODE : {
            FlowFinalNode& flowFinalNode = el.as<FlowFinalNode>();
            emitScope(emitter, flowFinalNode, data,
                        emitActivityNodeScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, flowFinalNode, data, "FlowFinalNode", 
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitActivityNodeFeatures);
            break;
        }
        case ElementType::FORK_NODE : {
            ForkNode& forkNode = el.as<ForkNode>();
            emitScope(emitter, forkNode, data,
                        emitActivityNodeScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, forkNode, data, "ForkNode", 
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitActivityNodeFeatures);
            break;
        }
        case ElementType::GENERALIZATION : {
            Generalization& generalization = el.as<Generalization>();
            emitScope(emitter, generalization, data,
                        emitGeneralizationScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, generalization, data, "Generalization",
                        emitElementFeatures,
                        emitGeneralizationFeatures);
            break;
        }
        case ElementType::GENERALIZATION_SET : {
            GeneralizationSet& generalizationSet = el.as<GeneralizationSet>();
            emitScope(emitter, generalizationSet, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, generalizationSet, data, "GeneralizationSet",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitParameterableElementFeatures,
                        emitGeneralizationSetFeatures);
            break;
        }
        case ElementType::INITIAL_NODE : {
            InitialNode& initialNode = el.as<InitialNode>();
            emitScope(emitter, initialNode, data,
                        emitActivityNodeScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, initialNode, data, "InitialNode", 
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitActivityNodeFeatures);
            break;
        }
        case ElementType::INPUT_PIN : {
            InputPin& inputPin = el.as<InputPin>();
            emitScope(emitter, inputPin, data,
                        emitActivityNodeScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, inputPin, data, "InputPin", 
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitActivityNodeFeatures,
                        emitTypedElementFeatures,
                        emitObjectNodeFeatures,
                        emitMultiplicityElementFeatures,
                        emitPinFeatures);
            break;
        }
        case ElementType::INSTANCE_SPECIFICATION : {
            InstanceSpecification& inst = el.as<InstanceSpecification>();
            emitScope(emitter, inst, data, 
                        emitPackageableElementScope, 
                        emitParameterableElementScope,
                        emitNamedElementScope, 
                        emitElementScope);
            emitElementTypeAndData(emitter, inst, data, "InstanceSpecification", 
                        emitElementFeatures, 
                        emitNamedElementDeploymentTargetFeatures, 
                        emitParameterableElementFeatures,
                        emitDeploymentTargetFeatures,
                        emitInstanceSpecificationFeatures);
            break;
        }
        case ElementType::INSTANCE_VALUE : {
            InstanceValue& instanceValue = el.as<InstanceValue>();
            emitScope(emitter, instanceValue, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, instanceValue, data, "InstanceValue",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitTypedElementFeatures,
                        emitParameterableElementFeatures,
                        emitInstanceValueFeatures);
            break;
        }
        case ElementType::INTERFACE_UML : {
            Interface& interface = el.as<Interface>();
            emitScope(emitter, interface, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, interface, data, "Interface", 
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitNamespaceFeatures,
                        emitTemplateableElementFeatures,
                        emitParameterableElementFeatures,
                        emitClassifierFeatures,
                        emitInterfaceFeatures);
            break;
        }
        case ElementType::INTERFACE_REALIZATION : {
            InterfaceRealization& interfaceRealization = el.as<InterfaceRealization>();
            emitScope(emitter, interfaceRealization, data,
                        emitInterfaceRealizationScope,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, interfaceRealization, data, "InterfaceRealization",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitParameterableElementFeatures,
                        // emitDependencyFeatures,
                        // TODO emitAbstractionFeatures;
                        emitInterfaceRealizationFeatures);
            break;
        }
        case ElementType::JOIN_NODE : {
            JoinNode& joinNode = el.as<JoinNode>();
            emitScope(emitter, joinNode, data,
                        emitActivityNodeScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, joinNode, data, "JoinNode", 
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitActivityNodeFeatures,
                        emitJoinNodeFeatures);
            break;
        }
        case ElementType::LITERAL_BOOL : {
            LiteralBool& literalBool = el.as<LiteralBool>();
            emitScope(emitter, literalBool, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, literalBool, data, "LiteralBool",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitParameterableElementFeatures,
                        emitTypedElementFeatures,
                        emitLiteralBoolFeatures);
            break;
        }
        case ElementType::LITERAL_INT : {
            LiteralInt& literalInt = el.as<LiteralInt>();
            emitScope(emitter, literalInt, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, literalInt, data, "LiteralInt",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitParameterableElementFeatures,
                        emitTypedElementFeatures,
                        emitLiteralIntFeatures);
            break;
        }
        case ElementType::LITERAL_NULL : {
            LiteralNull& literalNull = el.as<LiteralNull>();
            emitScope(emitter, literalNull, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, literalNull, data, "LiteralNull",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitParameterableElementFeatures,
                        emitTypedElementFeatures);
            break;
        }
        case ElementType::LITERAL_REAL : {
            LiteralReal& literalReal = el.as<LiteralReal>();
            emitScope(emitter, literalReal, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, literalReal, data, "LiteralReal",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitParameterableElementFeatures,
                        emitTypedElementFeatures,
                        emitLiteralRealFeatures);
            break;
        }
        case ElementType::LITERAL_STRING : {
            LiteralString& literalString = el.as<LiteralString>();
            emitScope(emitter, literalString, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, literalString, data, "LiteralString",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitParameterableElementFeatures,
                        emitTypedElementFeatures,
                        emitLiteralStringFeatures);
            break;
        }
        case ElementType::LITERAL_UNLIMITED_NATURAL : {
            LiteralUnlimitedNatural& literalUnlimitedNatural = el.as<LiteralUnlimitedNatural>();
            emitScope(emitter, literalUnlimitedNatural, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, literalUnlimitedNatural, data, "LiteralUnlimitedNatural",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitParameterableElementFeatures,
                        emitTypedElementFeatures,
                        emitLiteralUnlimitedNaturalFeatures);
            break;
        }
        case ElementType::MERGE_NODE : {
            MergeNode& mergeNode = el.as<MergeNode>();
            emitScope(emitter, mergeNode, data,
                        emitActivityNodeScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, mergeNode, data, "MergeNode", 
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitActivityNodeFeatures);
            break;
        }
        case ElementType::MANIFESTATION : {
            Manifestation& manifestation = el.as<Manifestation>();
            emitScope(emitter, manifestation, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, manifestation, data, "Manifestation", 
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitParameterableElementFeatures,
                        emitManifestationFeatures);
            break;
        }
        case ElementType::MODEL : {
            Model& model = el.as<Model>();
            emitScope(emitter, model, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, model, data, "Model", 
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitNamespaceFeatures,
                        emitParameterableElementFeatures,
                        emitTemplateableElementFeatures,
                        emitPackageFeatures,
                        emitModelFeatures);
            break;
        }
        case ElementType::OBJECT_FLOW : {
            ObjectFlow& objectFlow = el.as<ObjectFlow>();
            emitScope(emitter, objectFlow, data,
                        emitActivityEdgeScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, objectFlow, data, "ObjectFlow",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitActivityEdgeFeatures,
                        emitObjectFlowFeatures);
            break;
        }
        case ElementType::OPAQUE_ACTION : {
            OpaqueAction& opaqueAction = el.as<OpaqueAction>();
            emitScope(emitter, opaqueAction, data,
                        emitActivityNodeScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, opaqueAction, data, "OpaqueAction",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitActivityNodeFeatures,
                        emitExecutableNodeFeatures,
                        emitActionFeatures,
                        emitOpaqueActionFeatures);
            break;
        }
        case ElementType::OPAQUE_BEHAVIOR : {
            OpaqueBehavior& opaqueBehavior = el.as<OpaqueBehavior>();
            emitScope(emitter, opaqueBehavior, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, opaqueBehavior, data, "OpaqueBehavior", 
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitNamespaceFeatures,
                        emitParameterableElementFeatures,
                        emitTemplateableElementFeatures,
                        emitClassifierFeatures,
                        emitStructuredClassifierFeatures,
                        emitBehavioredClassifierFeatures,
                        emitClassFeatures,
                        emitBehaviorFeatures,
                        emitOpaqueBehaviorFeatures);
            break;
        }
        case ElementType::OPERATION : {
            Operation& operation = el.as<Operation>();
            emitScope(emitter, operation, data,
                        emitOperationScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, operation, data, "Operation", 
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitParameterableElementFeatures,
                        emitTemplateableElementFeatures,
                        emitFeatureFeatures,
                        emitBehavioralFeatureFeatures);
            break;
        }
        case ElementType::OUTPUT_PIN : {
            OutputPin& outputPin = el.as<OutputPin>();
            emitScope(emitter, outputPin, data,
                        emitActivityNodeScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, outputPin, data, "OutputPin", 
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitActivityNodeFeatures,
                        emitTypedElementFeatures,
                        emitObjectNodeFeatures,
                        emitMultiplicityElementFeatures,
                        emitPinFeatures);
            break;
        }
        case ElementType::PACKAGE : {
            Package& package = el.as<Package>();
            emitScope(emitter, package, data, 
                        emitPackageableElementScope, 
                        emitParameterableElementScope, 
                        emitNamedElementScope, 
                        emitElementScope);
            emitElementTypeAndData(emitter, package, data, "Package",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitNamespaceFeatures,
                        emitTemplateableElementFeatures,
                        emitParameterableElementFeatures,
                        emitPackageFeatures);
            break;
        }
        case ElementType::PACKAGE_IMPORT : {
            PackageImport& packageImport = el.as<PackageImport>();
            emitScope(emitter, packageImport, data,
                        emitPackageImportScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, packageImport, data, "PackageImport",
                        emitPackageImportFeatures,
                        emitElementFeatures);
            break;
        }
        case ElementType::PACKAGE_MERGE : {
            PackageMerge& packageMerge = el.as<PackageMerge>();
            emitScope(emitter, packageMerge, data, emitPackageMergeScope, emitElementScope);
            emitElementTypeAndData(emitter, packageMerge, data, "PackageMerge", emitElementFeatures, emitPackageMergeFeatures);
            break;
        }
        case ElementType::PARAMETER : {
            Parameter& parameter = el.as<Parameter>();
            emitScope(emitter, parameter, data,
                        emitParameterScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, parameter, data, "Parameter", 
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitTypedElementFeatures,
                        emitMultiplicityElementFeatures,
                        emitParameterFeatures);
            break;
        }
        case ElementType::PARAMETER_SET : {
            ParameterSet& parameterSet = el.as<ParameterSet>();
            emitScope(emitter, parameterSet, data, 
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, parameterSet, data, "ParameterSet",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitParameterSetFeatures);
            break;
        }
        case ElementType::PORT : {
            Port& port = el.as<Port>();
            emitScope(emitter, port, data, 
                        emitPropertyScope, 
                        emitParameterableElementScope,
                        emitNamedElementScope, 
                        emitElementScope);
            emitElementTypeAndData(emitter, port, data, "Port",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitFeatureFeatures,
                        emitTypedElementFeatures,
                        emitStructuralFeatureFeatures,
                        emitMultiplicityElementFeatures,
                        emitParameterableElementFeatures,
                        emitPropertyFeatures,
                        emitPortFeatures);
            break;
        }
        case ElementType::PRIMITIVE_TYPE : {
            PrimitiveType& primitiveType = el.as<PrimitiveType>();
            emitScope(emitter, primitiveType, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, primitiveType, data, "PrimitiveType", 
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitNamespaceFeatures,
                        emitTemplateableElementFeatures,
                        emitParameterableElementFeatures,
                        emitClassifierFeatures,
                        emitDataTypeFeatures);
            break;
        }
        case ElementType::PROFILE : {
            Profile& profile = el.as<Profile>();
            emitScope(emitter, profile, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, profile, data, "Profile", 
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitNamespaceFeatures,
                        emitParameterableElementFeatures,
                        emitPackageFeatures);
            break;
        }
        case ElementType::PROFILE_APPLICATION : {
            ProfileApplication& profileApplication = el.as<ProfileApplication>();
            emitScope(emitter, profileApplication, data,
                        emitProfileApplicationScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, profileApplication, data, "ProfileApplication",
                        emitElementFeatures,
                        emitProfileApplicationFeatures);
            break;
        }
        case ElementType::PROPERTY : {
            Property& property = el.as<Property>();
            emitScope(emitter, property, data, 
                        emitPropertyScope, 
                        emitParameterableElementScope,
                        emitNamedElementScope, 
                        emitElementScope);
            emitElementTypeAndData(emitter, property, data, "Property",
                        emitElementFeatures,
                        emitNamedElementDeploymentTargetFeatures,
                        emitFeatureFeatures,
                        emitTypedElementFeatures,
                        emitStructuralFeatureFeatures,
                        emitMultiplicityElementFeatures,
                        emitParameterableElementFeatures,
                        emitDeploymentTargetFeatures,
                        emitPropertyFeatures);
            break;
        }
        case ElementType::RECEPTION : {
            Reception& reception = el.as<Reception>();
            emitScope(emitter, reception, data,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, reception, data, "Reception",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitFeatureFeatures,
                        emitBehavioralFeatureFeatures,
                        emitReceptionFeatures);
            break;
        }
        case ElementType::REDEFINABLE_TEMPLATE_SIGNATURE : {
            RedefinableTemplateSignature& redefinableTemplateSignature = el.as<RedefinableTemplateSignature>();
            emitScope(emitter, redefinableTemplateSignature, data,
                        emitRedefinableTemplateSignatureScope,
                        emitTemplateSignatureScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, redefinableTemplateSignature, data, "RedefinableTemplateSignature",
                        emitElementFeatures,
                        emitTemplateSignatureFeatures,
                        emitRedefinableTemplateSignatureFeatures);
            break;
        }
        case ElementType::REALIZATION : {
            Realization& realization = el.as<Realization>();
            emitScope(emitter, realization, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, realization, data, "Realization",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitParameterableElementFeatures,
                        emitDependencyFeatures);
                        // TODO emitAbstractionFeatures;
            break;
        }
        case ElementType::SIGNAL : {
            Signal& signal = el.as<Signal>();
            emitScope(emitter, signal, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, signal, data, "Signal", 
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitParameterableElementFeatures,
                        emitNamespaceFeatures,
                        emitTemplateableElementFeatures,
                        emitClassifierFeatures,
                        emitSignalFeatures);
            break;
        }
        case ElementType::SLOT : {
            Slot& slot = el.as<Slot>();
            emitScope(emitter, slot, data,
                        emitSlotScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, slot, data, "Slot", 
                        emitElementFeatures,
                        emitSlotFeatures);
            break;
        }
        case ElementType::STEREOTYPE : {
            Stereotype& stereotype = el.as<Stereotype>();
            emitScope(emitter, stereotype, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, stereotype, data, "Stereotype",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitNamespaceFeatures,
                        emitParameterableElementFeatures,
                        emitTemplateableElementFeatures,
                        emitClassifierFeatures,
                        emitStructuredClassifierFeatures,
                        emitBehavioredClassifierFeatures,
                        emitClassFeatures,
                        emitStereotypeFeatures);
            break;
        }
        case ElementType::TEMPLATE_BINDING : {
            TemplateBinding& templateBinding = el.as<TemplateBinding>();
            emitScope(emitter, templateBinding, data,
                        emitTemplateBindingScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, templateBinding, data, "TemplateBinding",
                        emitElementFeatures,
                        emitTemplateBindingFeatures);
            break;
        }
        case ElementType::TEMPLATE_PARAMETER : {
            TemplateParameter& templateParameter = el.as<TemplateParameter>();
            emitScope(emitter, templateParameter, data, 
                        emitTemplateParameterScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, templateParameter, data, "TemplateParameter",
                        emitElementFeatures,
                        emitTemplateParameterFeatures);
            break;
        }
        case ElementType::TEMPLATE_PARAMETER_SUBSTITUTION : {
            TemplateParameterSubstitution& templateParameterSubstitution = el.as<TemplateParameterSubstitution>();
            emitScope(emitter, templateParameterSubstitution, data,
                        emitTemplateParameterSubstitutionScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, templateParameterSubstitution, data, "TemplateParameterSubstitution", 
                        emitElementFeatures,
                        emitTemplateParameterSubstitutionFeatures);
            break;
        }
        case ElementType::TEMPLATE_SIGNATURE : {
            TemplateSignature& templateSignature = el.as<TemplateSignature>();
            emitScope(emitter, templateSignature, data, emitTemplateSignatureScope, emitElementScope);
            emitElementTypeAndData(emitter, templateSignature, data, "TemplateSignature",
                        emitElementFeatures,
                        emitTemplateSignatureFeatures);
            break;
        }
        case ElementType::USAGE : {
            Usage& usage = el.as<Usage>();
            emitScope(emitter, usage, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, usage, data, "Usage",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitParameterableElementFeatures,
                        emitDependencyFeatures);
            break;
        }
        case ElementType::VALUE_PIN : {
            ValuePin& valuePin = el.as<ValuePin>();
            emitScope(emitter, valuePin, data,
                        emitActivityNodeScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, valuePin, data, "ValuePin", 
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitActivityNodeFeatures,
                        emitTypedElementFeatures,
                        emitObjectNodeFeatures,
                        emitMultiplicityElementFeatures,
                        emitPinFeatures,
                        emitValuePinFeatures);
            break;
        }
        default : {
            throw SerializationError("No strategy to serialize element of type " + Element::elementTypeToString(el.getElementType()));
        }
    }
}

void emitActionFeatures(YAML::Emitter& emitter, Action& action, EmitterData& data) {
    emitCompositeSet<Constraint>(emitter, action, data, "localPreconditions", &Action::getLocalPreconditions);
    emitCompositeSet<Constraint>(emitter, action, data, "localPostconditions", &Action::getLocalPostconditions);
}

void emitActionInputPinFeatures(YAML::Emitter& emitter, ActionInputPin& actionInputPin, __attribute__((unused)) EmitterData& data) {
    emitSingleton(emitter, actionInputPin, "fromAction", &ActionInputPin::getFromAction);
}

void emitActivityFeatures(YAML::Emitter& emitter, Activity& activity, EmitterData& data) {
    emitCompositeSet<ActivityNode>(emitter, activity, data, "nodes", &Activity::getNodes);
    emitCompositeSet<ActivityEdge>(emitter, activity, data, "edges", &Activity::getEdges);
    emitCompositeSet<ActivityPartition>(emitter, activity, data, "partitions", &Activity::getPartitions);
}

void emitActivityEdgeFeatures(YAML::Emitter& emitter, ActivityEdge& activityEdge, __attribute__((unused)) EmitterData& data) {
    emitSingleton(emitter, activityEdge, "target", &ActivityEdge::getTarget);
    emitSingleton(emitter, activityEdge, "source", &ActivityEdge::getSource);
    emitSingleton(emitter, activityEdge, "guard", &ActivityEdge::getGuard);
    emitSingleton(emitter, activityEdge, "weight", &ActivityEdge::getWeight);
    emitSet<ActivityPartition>(emitter, activityEdge, "inPartitions", &ActivityEdge::getInPartitions);
}

void emitActivityNodeFeatures(YAML::Emitter& emitter, ActivityNode& activityNode, __attribute__((unused)) EmitterData& data) {
    emitSet<ActivityEdge>(emitter, activityNode, "incoming", &ActivityNode::getIncoming);
    emitSet<ActivityEdge>(emitter, activityNode, "outgoing", &ActivityNode::getOutgoing);
    emitSet<ActivityPartition>(emitter, activityNode, "inPartitions", &ActivityNode::getInPartitions);
}

void emitActivityParameterNodeFeatures(YAML::Emitter& emitter, ActivityParameterNode& activityParameterNode, __attribute__((unused)) EmitterData& data) {
    emitSingleton(emitter, activityParameterNode, "parameter", &ActivityParameterNode::getParameter);
}

void emitActivityPartitionFeatures(YAML::Emitter& emitter, ActivityPartition& activityPartition, EmitterData& data) {
    emitSet<ActivityNode>(emitter, activityPartition, "nodes", &ActivityPartition::getNodes);
    emitSet<ActivityNode>(emitter, activityPartition, "edges", &ActivityPartition::getEdges);
    emitCompositeSet<ActivityPartition>(emitter, activityPartition, data, "subPartitions", &ActivityPartition::getSubPartitions);
    emitSingleton(emitter, activityPartition, "represents", &ActivityPartition::getRepresents);
}

void emitArtifactFeatures(YAML::Emitter& emitter, Artifact& artifact, EmitterData& data) {
    emitCompositeSet<Property>(emitter, artifact, data, "ownedAttributes", &Artifact::getOwnedAttributes);
    emitCompositeSet<Operation>(emitter, artifact, data, "ownedOperations", &Artifact::getOwnedOperations);
    emitCompositeSet<Artifact>(emitter, artifact, data, "nestedArtifacts", &Artifact::getNestedArtifacts);
    emitCompositeSet<Manifestation>(emitter, artifact, data, "manifestations", &Artifact::getManifestations);
}

void emitAssociationFeatures(YAML::Emitter& emitter, Association& association, EmitterData& data) {
    emitSet<Property>(emitter, association, "memberEnds", &Association::getMemberEnds, &Association::getOwnedEnds);
    emitCompositeSet<Property>(emitter, association, data, "ownedEnds", &Association::getOwnedEnds, &Association::getNavigableOwnedEnds);
    emitCompositeSet<Property>(emitter, association, data, "navigableOwnedEnds", &Association::getNavigableOwnedEnds);
}

void emitBehaviorFeatures(YAML::Emitter& emitter, Behavior& behavior, EmitterData& data) {
    emitSingleton<BehavioralFeature>(emitter, behavior, "specification", &Behavior::getSpecification);
    emitCompositeSet<Parameter>(emitter, behavior, data, "ownedParameters", &Behavior::getOwnedParameters);
}

void emitBehavioralFeatureFeatures(YAML::Emitter& emitter, BehavioralFeature& behavioralFeature, EmitterData& data) {
    if (behavioralFeature.getConcurrency() != CallConcurrencyKind::Sequential) {
        emitter << "concurrency" << YAML::Value;
        switch (behavioralFeature.getConcurrency()) {
            case CallConcurrencyKind::Concurrent : {
                emitter << "concurrent";
                break;
            }
            case CallConcurrencyKind::Guarded : {
                emitter << "guarded";
                break;
            }
        }
    }
    emitSet<Type>(emitter, behavioralFeature, "raisedExceptions", &BehavioralFeature::getRaisedExceptions);
    emitSet<Behavior>(emitter, behavioralFeature, "methods", &BehavioralFeature::getMethods);
    emitCompositeSet<Parameter>(emitter, behavioralFeature, data, "ownedParameters", &BehavioralFeature::getOwnedParameters);
    emitCompositeSet<ParameterSet>(emitter, behavioralFeature, data, "ownedParameterSets", &BehavioralFeature::getOwnedParameterSets);
}

void emitBehavioredClassifierFeatures(YAML::Emitter& emitter, BehavioredClassifier& behavioredClassifier, EmitterData& data) {
    emitOwnedSingleton<Behavior>(emitter, behavioredClassifier, data, "classifierBehavior", &BehavioredClassifier::getClassifierBehavior);
    emitCompositeSet<Behavior>(emitter, behavioredClassifier, data, "ownedBehaviors", &Behavior::getOwnedBehaviors, &BehavioredClassifier::getClassifierBehavior);
    emitCompositeSet<InterfaceRealization>(emitter, behavioredClassifier, data, "interfaceRealizations", &BehavioredClassifier::getInterfaceRealizations);
}

void emitCallActionFeatures(YAML::Emitter& emitter, CallAction& callAction, EmitterData& data) {
    emitCompositeSet<OutputPin>(emitter, callAction, data, "results", &CallAction::getResults);
}

void emitCallBehaviorActionFeatures(YAML::Emitter& emitter, CallBehaviorAction& callBehaviorAction, __attribute__((unused)) EmitterData& data) {
    emitSingleton(emitter, callBehaviorAction, "behavior", &CallBehaviorAction::getBehavior);
}

void emitClassFeatures(YAML::Emitter& emitter, Class& clazz, EmitterData& data) {
    emitCompositeSet<Classifier>(emitter, clazz, data, "nestedClassifiers", &Class::getNestedClassifiers);
    emitCompositeSet<Property>(emitter, clazz, data, "ownedAttributes", &Class::getOwnedAttributes);
    emitCompositeSet<Operation>(emitter, clazz, data, "ownedOperations", &Class::getOwnedOperations);
    emitCompositeSet<Reception>(emitter, clazz, data, "ownedReceptions", &Class::getOwnedReceptions);
}

void emitClassifierFeatures(YAML::Emitter& emitter, Classifier& classifier, EmitterData& data) {
    emitCompositeSet<Generalization>(emitter, classifier, data, "generalizations", &Classifier::getGeneralizations);
    emitSet<GeneralizationSet>(emitter, classifier, "powertypeExtent", &Classifier::getPowerTypeExtent);
}

void emitClassifierTemplateParameter(YAML::Emitter& emitter, ClassifierTemplateParameter& classifierTemplateParameter, __attribute__((unused)) EmitterData& data) {
    emitSet<Classifier>(emitter, classifierTemplateParameter, "constrainingClassifiers", &ClassifierTemplateParameter::getConstrainingClassifiers);
}

void emitCommentFeatures(YAML::Emitter& emitter, Comment& comment, __attribute__((unused)) EmitterData& data) {
    emitSet<Element>(emitter, comment, "annotatedElements", &Comment::getAnnotatedElements);
    if (!comment.getBody().empty()) {
        emitter << "body" << YAML::Value << comment.getBody();
    }
}

void emitConnectorFeatures(YAML::Emitter& emitter, Connector& connector, EmitterData& data) {
    emitCompositeSet<ConnectorEnd>(emitter, connector, data, "ends", &Connector::getEnds);
    //emitSet<Connector>(emitter, connector, "redefinedConnectors", &Connector::getRedefinedConnecots);
    emitSet<Behavior>(emitter, connector, "contracts", &Connector::getContracts);
    emitSingleton<Association>(emitter, connector, "type", &Connector::getType);
}

void emitConnectorEndFeatures(YAML::Emitter& emitter, ConnectorEnd& connectorEnd, __attribute__((unused)) EmitterData& data) {
    emitSingleton<ConnectableElement>(emitter, connectorEnd, "role", &ConnectorEnd::getRole);
}

void emitConstraintFeatures(YAML::Emitter& emitter, Constraint& constraint, EmitterData& data) {
    emitSet<Element>(emitter, constraint, "constrainedElements", &Constraint::getConstrainedElements);
    emitOwnedSingleton<ValueSpecification>(emitter, constraint, data, "specification", &Constraint::getSpecification);
}

void emitDataTypeFeatures(YAML::Emitter& emitter, DataType& dataType, EmitterData& data) {
    emitCompositeSet<Property>(emitter, dataType, data, "ownedAttributes", &DataType::getOwnedAttributes);
    emitCompositeSet<Operation>(emitter, dataType, data, "ownedOperations", &DataType::getOwnedOperations);
}

void emitDecisionNodeFeatures(YAML::Emitter& emitter, DecisionNode& decisionNode, __attribute__((unused)) EmitterData& data) {
    emitSingleton(emitter, decisionNode, "decisionInput", &DecisionNode::getDecisionInput);
    emitSingleton(emitter, decisionNode, "decisionInputFlow", &DecisionNode::getDecisionInputFlow);
}

void emitDependencyFeatures(YAML::Emitter& emitter, Dependency& dependency, __attribute__((unused)) EmitterData& data) {
    emitSet<NamedElement>(emitter, dependency, "suppliers", &Dependency::getSuppliers);
    emitSet<NamedElement>(emitter, dependency, "clients", &Dependency::getClients);
}

void emitDeploymentFeatures(YAML::Emitter& emitter, Deployment& deployment, __attribute__((unused)) EmitterData& data) {
    emitSet<DeployedArtifact>(emitter, deployment, "deployedArtifacts", &Deployment::getDeployedArtifacts);
}

void emitDeploymentTargetFeatures(YAML::Emitter& emitter, DeploymentTarget& deploymentTarget, EmitterData& data) {
    emitCompositeSet<Deployment>(emitter, deploymentTarget, data, "deployments", &DeploymentTarget::getDeployments);
}

void emitElementFeatures(YAML::Emitter& emitter, Element& el, EmitterData& data) {
    emitter << "id" << YAML::Value << el.getID().string();
    emitCompositeSet<Comment>(emitter, el, data, "ownedComments", &Element::getOwnedComments);
    emitCompositeSet<InstanceSpecification>(emitter, el, data, "appliedStereotypes", &Element::getAppliedStereotypes);
}

template <class T>
void emitVisibilityKind(YAML::Emitter& emitter, T& elWithVisibility) {
    if (elWithVisibility.getVisibility() != VisibilityKind::PUBLIC) {
        emitter << "visibility" << YAML::Value;
        switch (elWithVisibility.getVisibility()) {
            case VisibilityKind::PACKAGE : {
                emitter << "package";
                break;
            }
            case VisibilityKind::PRIVATE : {
                emitter << "private";
                break;
            }
            case VisibilityKind::PROTECTED : {
                emitter << "protected";
                break;
            }
        }
    }
}

void emitElementImportFeatures(YAML::Emitter& emitter, ElementImport& elementImport, __attribute__((unused)) EmitterData& data) {
    emitSingleton<PackageableElement>(emitter, elementImport, "importedElement", &ElementImport::getImportedElement);
    if (!elementImport.getAlias().empty()) {
        emitter << "alias" << YAML::Value << elementImport.getAlias();
    }
    emitVisibilityKind(emitter, elementImport);
}

void emitEnumerationFeatures(YAML::Emitter& emitter, Enumeration& enumeration, EmitterData& data) {
    emitCompositeSet<EnumerationLiteral>(emitter, enumeration, data, "ownedLiterals", &Enumeration::getOwnedLiterals);
}

void emitExceptionHandlerFeatures(YAML::Emitter& emitter, ExceptionHandler& exceptionHandler, __attribute__((unused)) EmitterData& data) {
    emitSingleton(emitter, exceptionHandler, "handlerBody", &ExceptionHandler::getHandlerBody);
    emitSingleton(emitter, exceptionHandler, "exceptionInput", &ExceptionHandler::getExceptionInput);
    emitSet<Classifier>(emitter, exceptionHandler, "exceptionTypes", &ExceptionHandler::getExceptionTypes);
}

void emitExecutableNodeFeatures(YAML::Emitter& emitter, ExecutableNode& executableNode, EmitterData& data) {
    emitCompositeSet<ExceptionHandler>(emitter, executableNode, data, "handlers", &ExecutableNode::getHandlers);
}

void emitExpressionFeatures(YAML::Emitter& emitter, Expression& expression, EmitterData& data) {
    if (!expression.getSymbol().empty()) {
        emitter << "symbol" << YAML::Value << expression.getSymbol();
    }
    emitCompositeSet<ValueSpecification>(emitter, expression, data, "operands", &Expression::getOperands);
}

void emitExtensionFeatures(YAML::Emitter& emitter, Extension& extension, EmitterData& data) {
    emitSet<Property, Association>(emitter, extension, "memberEnds", &Association::getMemberEnds, &Association::getOwnedEnds);
    emitOwnedSingleton<ExtensionEnd>(emitter, extension, data, "ownedEnd", &Extension::getOwnedEnd);
    // if (extension.getMetaClass() != ElementType::NOT_SET) {
    //     emitter << "metaClass" << YAML::Value << Element::elementTypeToString(extension.getMetaClass());
    // }
}

void emitFeatureFeatures(YAML::Emitter& emitter, Feature& feature, __attribute__((unused)) EmitterData& data) {
    if (feature.isStatic()) {
        emitter << "isStatic" << YAML::Value << true;
    }
}

void emitGeneralizationFeatures(YAML::Emitter& emitter, Generalization& generalization, __attribute__((unused)) EmitterData& data) {
    emitSingleton<Classifier>(emitter, generalization, "general", &Generalization::getGeneral);
    emitSet<GeneralizationSet>(emitter, generalization, "generalizationSets", &Generalization::getGeneralizationSets);
}

void emitGeneralizationSetFeatures(YAML::Emitter& emitter, GeneralizationSet& generalizationSet, __attribute__((unused)) EmitterData& data) {
    emitter << "isCovering" << YAML::Value << generalizationSet.isCovering();
    emitter << "isDisjoint" << YAML::Value << generalizationSet.isDisjoint();
    emitSingleton<Classifier>(emitter, generalizationSet, "powertype", &GeneralizationSet::getPowerType);
    emitSet<Generalization>(emitter, generalizationSet, "generalizations", &GeneralizationSet::getGeneralizations);
}

void emitInstanceSpecificationFeatures(YAML::Emitter& emitter, InstanceSpecification& specification, EmitterData& data) {
    emitCompositeSet<Slot>(emitter, specification, data, "slots", &InstanceSpecification::getSlots);
    emitOwnedSingleton<ValueSpecification>(emitter, specification, data, "specification", &InstanceSpecification::getSpecification);
    emitSet<Classifier>(emitter, specification, "classifiers", &InstanceSpecification::getClassifiers);
}

void emitInstanceValueFeatures(YAML::Emitter& emitter, InstanceValue& instanceValue, __attribute__((unused)) EmitterData& data) {
    emitSingleton<InstanceSpecification>(emitter, instanceValue, "instance", &InstanceValue::getInstance);
}

void emitInterfaceFeatures(YAML::Emitter& emitter, Interface& interface, EmitterData& data) {
    emitCompositeSet<Property>(emitter, interface, data, "ownedAttributes", &Interface::getOwnedAttributes);
    emitCompositeSet<Operation>(emitter, interface, data, "ownedOperations", &Interface::getOwnedOperations);
    // emitCompositeSet<Reception>(emitter, interface, data, "ownedReception", &Interface::getOwnedReception);
    emitCompositeSet<Classifier>(emitter, interface, data, "nestedClassifiers", &Interface::getNestedClassifiers);
    // emitSet<Interface>(emitter, interface, data, "redefinedInterfaces", &Interface::getRedefinedInterfaces);
}

void emitInterfaceRealizationFeatures(YAML::Emitter& emitter, InterfaceRealization& interfaceRealization, __attribute__((unused)) EmitterData& data) {
    // emitSet<NamedElement, InterfaceRealization>(emitter, interfaceRealization, "clients", &Dependency::getClients, &InterfaceRealization::getImplementingClassifier);
    // emitSet<NamedElement, InterfaceRealization>(emitter, interfaceRealization, "suppliers", &Dependency::getSuppliers, &InterfaceRealization::getContract);
    emitSingleton<Interface>(emitter, interfaceRealization, "contract", &InterfaceRealization::getContract);
}

void emitInvocationActionFeatures(YAML::Emitter& emitter, InvocationAction& invocationAction, EmitterData& data) {
    emitCompositeSet<InputPin>(emitter, invocationAction, data, "arguments", &InvocationAction::getArguments);
    emitSingleton(emitter, invocationAction, "onPort", &InvocationAction::getOnPort);
}

void emitJoinNodeFeatures(YAML::Emitter& emitter, JoinNode& joinNode, EmitterData& data) {
    emitOwnedSingleton(emitter, joinNode, data, "joinSpec", &JoinNode::getJoinSpec);
}

void emitLiteralBoolFeatures(YAML::Emitter& emitter, LiteralBool& literalBool, __attribute__((unused)) EmitterData& data) {
    emitter << "value" << YAML::Value << literalBool.getValue();
}

void emitLiteralIntFeatures(YAML::Emitter& emitter, LiteralInt& literalInt, __attribute__((unused)) EmitterData& data) {
    emitter << "value" << YAML::Value << literalInt.getValue();
}

void emitLiteralRealFeatures(YAML::Emitter& emitter, LiteralReal& literalReal, __attribute__((unused)) EmitterData& data) {
    emitter << "value" << YAML::Value << literalReal.getValue();
}

void emitLiteralStringFeatures(YAML::Emitter& emitter, LiteralString& literalString, __attribute__((unused)) EmitterData& data) {
    if (!literalString.getValue().empty()) {
        emitter << "value" << YAML::Value << literalString.getValue();
    }
}

void emitLiteralUnlimitedNaturalFeatures(YAML::Emitter& emitter, LiteralUnlimitedNatural& literalUnlimitedNatural, __attribute__((unused)) EmitterData& data) {
    emitter << "value" << YAML::Value;
    if (literalUnlimitedNatural.isInfinite()) {
        emitter << "*";
    } else {
        emitter << literalUnlimitedNatural.getNumberValue();
    }
}

void emitManifestationFeatures(YAML::Emitter& emitter, Manifestation& manifestation, __attribute__((unused)) EmitterData& data) {
    emitSingleton(emitter, manifestation, "utilizedElement", &Manifestation::getUtilizedElement);
}

void emitModelFeatures(YAML::Emitter& emitter, Model& model, __attribute__((unused)) EmitterData& data) {
    if (!model.getViewpoint().empty()) {
        emitter << "viewpoint" << YAML::Value << model.getViewpoint();
    }
}

void emitMultiplicityElementFeatures(YAML::Emitter& emitter, MultiplicityElement& multiplicityElement, EmitterData& data) {
    if (multiplicityElement.isOrdered()) {
        emitter << "isOrdered" << YAML::Value << true;
    }
    if (!multiplicityElement.isUnique()) {
        emitter << "isUnique" << YAML::Value << false;
    }
    if (!emitOwnedSingleton<ValueSpecification>(emitter, multiplicityElement, data, "lowerValue", &MultiplicityElement::getLowerValue)) {
        if (multiplicityElement.getLower() != -1) {
            emitter << "lower" << YAML::Value << multiplicityElement.getLower();
        }
    }
    if (!emitOwnedSingleton<ValueSpecification>(emitter, multiplicityElement, data, "upperValue", &MultiplicityElement::getUpperValue)) {
        if (multiplicityElement.getUpper() != -1) {
            emitter << "lower" << YAML::Value << multiplicityElement.getUpper();
        }
    }
}

void emitNamedElementFeatures(YAML::Emitter& emitter, NamedElement& el, __attribute__((unused)) EmitterData& data) {
    if (!el.getName().empty()) {
        emitter << "name" << YAML::Value << el.getName();
    }
    emitVisibilityKind(emitter, el);
    emitSet<Dependency>(emitter, el, "clientDependencies", &NamedElement::getClientDependencies);
}

void emitNamedElementDeploymentTargetFeatures(YAML::Emitter& emitter, NamedElement& el, __attribute__((unused)) EmitterData& data) {
    if (!el.getName().empty()) {
        emitter << "name" << YAML::Value << el.getName();
    }
    emitVisibilityKind(emitter, el);
    // emitSet<Dependency>(emitter, el, "clientDependencies", &NamedElement::getClientDependencies, &DeploymentTarget::getDeployments);
    if (!el.getClientDependencies().empty()) {
        if (el.as<DeploymentTarget>().getDeployments().size() != el.getClientDependencies().size()) {
            emitSet<Dependency>(emitter, el, "clientDependencies", &NamedElement::getClientDependencies);
        }
    }
}

void emitNamespaceFeatures(YAML::Emitter& emitter, Namespace& nmspc, EmitterData& data) {
    emitCompositeSet<ElementImport>(emitter, nmspc, data, "elementImports", &Namespace::getElementImports);
    emitCompositeSet<PackageImport>(emitter, nmspc, data, "packageImports", &Namespace::getPackageImports);
    emitCompositeSet<Constraint>(emitter, nmspc, data, "ownedRules", &Namespace::getOwnedRules);
}

void emitObjectFlowFeatures(YAML::Emitter& emitter, ObjectFlow& objectFlow, __attribute__((unused)) EmitterData& data) {
    emitSingleton(emitter, objectFlow, "transformation", &ObjectFlow::getTransformation);
    emitSingleton(emitter, objectFlow, "selection", &ObjectFlow::getSelection);
}

void emitObjectNodeFeatures(YAML::Emitter& emitter, ObjectNode& objectNode, EmitterData& data) {
    emitSingleton(emitter, objectNode, "selection", &ObjectNode::getSelection);
    emitOwnedSingleton(emitter, objectNode, data, "upperBound", &ObjectNode::getUpperBound);
    if (objectNode.isControlType()) {
        emitter << "isControlType" << YAML::Value << true;
    }
    if (objectNode.getOrdering() != ObjectNodeOrderingKind::FIFO) {
        emitter << "ordering" << YAML::Value;
        switch (objectNode.getOrdering()) {
            case ObjectNodeOrderingKind::UNORDERED : {
                emitter << "unordered";
                break;
            }
            case ObjectNodeOrderingKind::ORDERED : {
                emitter << "ordered";
                break;
            }
            case ObjectNodeOrderingKind::LIFO : {
                emitter << "LIFO";
                break;
            }
            default : {
                throw SerializationError("Could not determine OrderingKind to emit ");
            }
        }
    }
}

void emitOpaqueActionFeatures(YAML::Emitter& emitter, OpaqueAction& opaqueAction, EmitterData& data) {
    emitCompositeSet<InputPin>(emitter, opaqueAction, data, "inputValues", &OpaqueAction::getInputValues);
    emitCompositeSet<OutputPin>(emitter, opaqueAction, data, "outputValues", &OpaqueAction::getOutputValues);
    emitCompositeSet<LiteralString>(emitter, opaqueAction, data, "bodies", &OpaqueAction::getBodies);
}

void emitOpaqueBehaviorFeatures(YAML::Emitter& emitter, OpaqueBehavior& opaqueBehavior, EmitterData& data) {
    emitCompositeSet<LiteralString>(emitter, opaqueBehavior, data, "bodies", &OpaqueBehavior::getBodies);
}

void emitPackageFeatures(YAML::Emitter& emitter, Package& package, EmitterData& data) {
    emitCompositeSet<PackageableElement>(emitter, package, data, "packagedElements", &Package::getPackagedElements, &Package::getOwnedStereotypes);
    emitCompositeSet<Stereotype>(emitter, package, data, "ownedStereotypes", &Package::getOwnedStereotypes);
    emitCompositeSet<PackageMerge>(emitter, package, data, "packageMerges", &Package::getPackageMerge);
    emitCompositeSet<ProfileApplication>(emitter, package, data, "profileApplications", &Package::getProfileApplications);
}

void emitPackageImportFeatures(YAML::Emitter& emitter, PackageImport& packageImport, __attribute__((unused)) EmitterData& data) {
    emitSingleton<Package>(emitter, packageImport, "importedPackage", &PackageImport::getImportedPackage);
    emitVisibilityKind(emitter, packageImport);
}

void emitPackageMergeFeatures(YAML::Emitter& emitter, PackageMerge& packageMerge, __attribute__((unused)) EmitterData& data) {
    emitSingleton<Package>(emitter, packageMerge, "mergedPackage", &PackageMerge::getMergedPackage);
}

void emitParameterFeatures(YAML::Emitter& emitter, Parameter& parameter, EmitterData& data) {
    if (parameter.getDirection() != ParameterDirectionKind::NONE) {
        emitter << "direction" << YAML::Value;
        switch (parameter.getDirection()) {
            case ParameterDirectionKind::IN_UML : {
                emitter << "in";
                break;
            }
            case ParameterDirectionKind::INOUT : {
                emitter << "inout";
                break;
            }
            case ParameterDirectionKind::OUT_UML : {
                emitter << "out";
                break;
            }
            case ParameterDirectionKind::RETURN : {
                emitter << "return";
                break;
            }
            default : {
                throw SerializationError(" bad direction");
            }
        }
    }
    if (parameter.isException()) {
        emitter << "isException" << YAML::Value << true;
    }
    if (parameter.isStream()) {
        emitter << "isStream" << YAML::Value << true;
    }
    emitOwnedSingleton<ValueSpecification>(emitter, parameter, data, "defaultValue", &Parameter::getDefaultValue);
    emitSet<ParameterSet>(emitter, parameter, "parameterSets", &Parameter::getParameterSets);
}

void emitParameterableElementFeatures(YAML::Emitter& emitter, ParameterableElement& parameterableElement, __attribute__((unused)) EmitterData& data) {
    if (!parameterableElement.getOwningTemplateParameter()) {
        emitSingleton<TemplateParameter>(emitter, parameterableElement, "templateParameter", &ParameterableElement::getTemplateParameter);
    }
}

void emitParameterSetFeatures(YAML::Emitter& emitter, ParameterSet& parameterSet, EmitterData& data) {
    emitSet<Parameter>(emitter, parameterSet, "parameters", &ParameterSet::getParameters);
    emitCompositeSet<Constraint>(emitter, parameterSet, data, "conditions", &ParameterSet::getConditions);
}

void emitPinFeatures(YAML::Emitter& emitter, Pin& pin, __attribute__((unused)) EmitterData& data) {
    if (pin.isControl()) {
        emitter << "isControl" << YAML::Value << true;
    }
}

void emitPortFeatures(YAML::Emitter& emitter, Port& port, __attribute__((unused)) EmitterData& data) {
    if (port.isBehavior()) {
        emitter << "isBehavior" << YAML::Value << true;
    }
    if (port.isConjugated()) {
        emitter << "isConjugated" << YAML::Value << true;
    }
    if (!port.isService()) {
        emitter << "isService" << YAML::Value << false;
    }
}

void emitProfileApplicationFeatures(YAML::Emitter& emitter, ProfileApplication& profileApplication, __attribute__((unused)) EmitterData& data) {
    emitSingleton<Profile>(emitter, profileApplication, "appliedProfile", &ProfileApplication::getAppliedProfile);
}

void emitPropertyFeatures(YAML::Emitter& emitter, Property& property, EmitterData& data) {
    if (property.getAggregation() != AggregationKind::NONE) {
        emitter << "aggregation" << YAML::Value;
        switch (property.getAggregation()) {
            case AggregationKind::COMPOSITE : {
                emitter << "composite";
                break;
            }
            case AggregationKind::SHARED : {
                emitter << "shared";
                break;
            }
        }
    }
    if (property.isComposite()) {
        emitter << "isComposite" << YAML::Value << true;
    }
    emitOwnedSingleton<ValueSpecification>(emitter, property, data, "defaultValue", &Property::getDefaultValue);
    if (!property.getOwningAssociation()) {
        emitSingleton<Association>(emitter, property, "association", &Property::getAssociation);
    }
    emitSet<Property>(emitter, property, "redefinedProperties", &Property::getRedefinedProperties);
    emitSet<Property>(emitter, property, "subsettedProperties", &Property::getSubsettedProperties);
}

void emitReceptionFeatures(YAML::Emitter& emitter, Reception& reception, __attribute__((unused)) EmitterData& data) {
    emitSingleton<Signal>(emitter, reception, "signal", &Reception::getSignal);
}

void emitRedefinableTemplateSignatureFeatures(YAML::Emitter& emitter, RedefinableTemplateSignature& redefinableTemplateSignature, __attribute__((unused)) EmitterData& data) {
    emitSet<RedefinableTemplateSignature>(emitter, redefinableTemplateSignature, "extendedSignatures", &RedefinableTemplateSignature::getExtendedSignatures);
}

void emitSignalFeatures(YAML::Emitter& emitter, Signal& signal, EmitterData& data) {
    emitCompositeSet<Property>(emitter, signal, data, "ownedAttributes", &Signal::getOwnedAttributes);
}

void emitSlotFeatures(YAML::Emitter& emitter, Slot& slot, EmitterData& data) {
    emitCompositeSet<ValueSpecification>(emitter, slot, data, "values", &Slot::getValues);
    emitSingleton<StructuralFeature>(emitter, slot, "definingFeature", &Slot::getDefiningFeature);
}

void emitStereotypeFeatures(YAML::Emitter& emitter, Stereotype& stereotype, __attribute__((unused)) EmitterData& data) {
    emitSingleton<Profile>(emitter, stereotype, "profile", &Stereotype::getProfile);
}

void emitStructuralFeatureFeatures(YAML::Emitter& emitter, StructuralFeature& structuralFeature, __attribute__((unused)) EmitterData& data) {
    if (structuralFeature.isReadOnly()) {
        emitter << "isReadOnly" << YAML::Value << true;
    }
}

void emitStructuredClassifierFeatures(YAML::Emitter& emitter, StructuredClassifier& structuredClassifier, EmitterData& data) {
    emitCompositeSet<Connector>(emitter, structuredClassifier, data, "ownedConnectors", &StructuredClassifier::getOwnedConnectors);
}

void emitTemplateableElementFeatures(YAML::Emitter& emitter, TemplateableElement& templateableElement, EmitterData& data) {
    emitOwnedSingleton<TemplateSignature>(emitter, templateableElement, data, "ownedTemplateSignature", &TemplateableElement::getOwnedTemplateSignature);
    emitCompositeSet<TemplateBinding>(emitter, templateableElement, data, "templateBindings", &TemplateableElement::getTemplateBindings);
}

void emitTemplateBindingFeatures(YAML::Emitter& emitter, TemplateBinding& templateBinding, EmitterData& data) {
    emitCompositeSet<TemplateParameterSubstitution>(emitter, templateBinding, data, "parameterSubstitutions", &TemplateBinding::getParameterSubstitutions);
    emitSingleton<TemplateSignature>(emitter, templateBinding, "signature", &TemplateBinding::getSignature);
}

void emitTemplateParameterFeatures(YAML::Emitter& emitter, TemplateParameter& templateParameter, EmitterData& data) {
    if (!emitOwnedSingleton<ParameterableElement>(emitter, templateParameter, data, "ownedParameteredElement", &TemplateParameter::getOwnedParameteredElement)) {
        emitSingleton<ParameterableElement>(emitter, templateParameter, "parameteredElement", &TemplateParameter::getParameteredElement);
    }
    if (!emitOwnedSingleton<ParameterableElement>(emitter, templateParameter, data, "ownedDefault", &TemplateParameter::getOwnedDefault)) {
        emitSingleton<ParameterableElement>(emitter, templateParameter, "default", &TemplateParameter::getDefault);
    }
}

void emitTemplateParameterSubstitutionFeatures(YAML::Emitter& emitter, TemplateParameterSubstitution& templateParameterSubstitution, EmitterData& data) {
    emitSingleton<TemplateParameter>(emitter, templateParameterSubstitution, "formal", &TemplateParameterSubstitution::getFormal);
    if (!emitOwnedSingleton<ParameterableElement>(emitter, templateParameterSubstitution, data, "ownedActual", &TemplateParameterSubstitution::getOwnedActual)) {
        emitSingleton<ParameterableElement>(emitter, templateParameterSubstitution, "actual", &TemplateParameterSubstitution::getActual);
    }
}

void emitTemplateSignatureFeatures(YAML::Emitter& emitter, TemplateSignature& signature, EmitterData& data) {
    emitCompositeSet<TemplateParameter>(emitter, signature, data, "ownedParameters", &TemplateSignature::getOwnedParameters);
    emitSet<TemplateParameter>(emitter, signature, "parameters", &TemplateSignature::getParameters, &TemplateSignature::getOwnedParameters);
}

void emitTypedElementFeatures(YAML::Emitter& emitter, TypedElement& typedElement, __attribute__((unused)) EmitterData& data) {
    emitSingleton<Type>(emitter, typedElement, "type", &TypedElement::getType);
}

void emitValuePinFeatures(YAML::Emitter& emitter, ValuePin& valuePin, EmitterData& data) {
    emitOwnedSingleton(emitter, valuePin, data, "value", &ValuePin::getValue);
}

bool emitActivityEdgeScope(YAML::Emitter& emitter, ActivityEdge& activityEdge, __attribute__((unused)) EmitterData& data) {
    return emitSingleton(emitter, activityEdge, "activity", &ActivityEdge::getActivity);
}

bool emitActivityNodeScope(YAML::Emitter& emitter, ActivityNode& activityNode, __attribute__((unused)) EmitterData& data) {
    return emitSingleton(emitter, activityNode, "activity", &ActivityNode::getActivity);
}

bool emitActivityGroupScope(YAML::Emitter& emitter, ActivityGroup& activityGroup, __attribute__((unused)) EmitterData& data) {
    return emitSingleton(emitter, activityGroup, "inActivity", &ActivityGroup::getInActivity);
}

bool emitActivityPartitionScope(YAML::Emitter& emitter, ActivityPartition& activityPartition, __attribute__((unused)) EmitterData& data) {
    return emitSingleton(emitter, activityPartition, "superPartition", &ActivityPartition::getSuperPartition);
}

bool emitConstraintScope(YAML::Emitter& emitter, Constraint& constraint, __attribute__((unused)) EmitterData& data) {
    return emitSingleton<Namespace>(emitter, constraint, "context", &Constraint::getContext);
}

bool emitDeploymentScope(YAML::Emitter& emitter, Deployment& deployment, __attribute__((unused)) EmitterData& data) {
    return emitSingleton(emitter, deployment, "location", &Deployment::getLocation);
}

bool emitElementScope(YAML::Emitter& emitter, Element& el, __attribute__((unused)) EmitterData& data) {
    return emitSingleton<Element>(emitter, el, "owner", &Element::getOwner);
}

bool emitElementImportScope(YAML::Emitter& emitter, ElementImport& elementImport, __attribute__((unused)) EmitterData& data) {
    return emitSingleton<Namespace>(emitter, elementImport, "importingNamespace", &ElementImport::getImportingNamespace);
}

bool emitEnumerationLiteralScope(YAML::Emitter& emitter, EnumerationLiteral& literal, __attribute__((unused)) EmitterData& data) {
    return emitSingleton<Enumeration>(emitter, literal, "enumeration", &EnumerationLiteral::getEnumeration);
}

bool emitExceptionHandlerScope(YAML::Emitter& emitter, ExceptionHandler& exceptionHandler, __attribute__((unused)) EmitterData& data) {
    return emitSingleton(emitter, exceptionHandler, "protectedNode", &ExceptionHandler::getProtectedNode);
}

bool emitGeneralizationScope(YAML::Emitter& emitter, Generalization& generalization, __attribute__((unused)) EmitterData& data) {
    return emitSingleton<Classifier>(emitter, generalization, "specific", &Generalization::getSpecific);
}

bool emitInterfaceRealizationScope(YAML::Emitter& emitter, InterfaceRealization& interfaceRealization, __attribute__((unused)) EmitterData& data) {
    return emitSingleton<BehavioredClassifier>(emitter, interfaceRealization, "implementingClassifier", &InterfaceRealization::getImplementingClassifier);
}

bool emitNamedElementScope(YAML::Emitter& emitter, NamedElement& namedElement, __attribute__((unused)) EmitterData& data) {
    return emitSingleton<Namespace>(emitter, namedElement, "namespace", &NamedElement::getNamespace);
}

bool emitOperationScope(YAML::Emitter& emitter, Operation& operation, __attribute__((unused)) EmitterData& data) {
    if (emitSingleton<Class>(emitter, operation, "class", &Operation::getClass)) {
        return true;
    } else if (emitSingleton<DataType>(emitter, operation, "dataType", &Operation::getDataType)) {
        return true;
    } else if (emitSingleton<Interface>(emitter, operation, "interface", &Operation::getInterface)) {
        return true;
    }
    return false;
}

bool emitPackageableElementScope(YAML::Emitter& emitter, PackageableElement& el, __attribute__((unused)) EmitterData& data) {
    return emitSingleton<Package>(emitter, el, "owningPackage", &PackageableElement::getOwningPackage);
}

bool emitPackageImportScope(YAML::Emitter& emitter, PackageImport& packageImport, __attribute__((unused)) EmitterData& data) {
    return emitSingleton<Namespace>(emitter, packageImport, "importingNamespace", &PackageImport::getImportingNamespace);
}

bool emitPackageMergeScope(YAML::Emitter& emitter, PackageMerge& packageMerge, __attribute__((unused)) EmitterData& data) {
    return emitSingleton<Package>(emitter, packageMerge, "receivingPackage", &PackageMerge::getReceivingPackage);
}

bool emitParameterScope(YAML::Emitter& emitter, Parameter& parameter, __attribute__((unused)) EmitterData& data) {
    return emitSingleton<Operation>(emitter, parameter, "operation", &Parameter::getOperation);
}

bool emitParameterableElementScope(YAML::Emitter& emitter, ParameterableElement& el, __attribute__((unused)) EmitterData& data) {
    return emitSingleton<TemplateParameter>(emitter, el, "owningTemplateParameter", &ParameterableElement::getOwningTemplateParameter);
}

bool emitProfileApplicationScope(YAML::Emitter& emitter, ProfileApplication& profileApplication, __attribute__((unused)) EmitterData& data) {
    return emitSingleton<Package>(emitter, profileApplication, "applyingPackage", &ProfileApplication::getApplyingPackage);
}

bool emitPropertyScope(YAML::Emitter& emitter, Property& property, __attribute__((unused)) EmitterData& data) {
    if (emitSingleton<Class>(emitter, property, "class", &Property::getClass)) {
        return true;
    } else if (emitSingleton<DataType>(emitter, property, "dataType", &Property::getDataType)) {
        return true;
    } else if (emitSingleton<Interface>(emitter, property, "interface", &Property::getInterface)) {
        return true;
    } else if (emitSingleton<Association>(emitter, property, "owningAssociation", &Property::getOwningAssociation)) {
        return true;
    }
    return false;
}

bool emitRedefinableTemplateSignatureScope(YAML::Emitter& emitter, RedefinableTemplateSignature& redefinableTemplateSignature, __attribute__((unused)) EmitterData& data) {
    return emitSingleton<Classifier>(emitter, redefinableTemplateSignature, "classifier", &RedefinableTemplateSignature::getClassifier);
}

bool emitSlotScope(YAML::Emitter& emitter, Slot& slot, __attribute__((unused)) EmitterData& data) {
    return emitSingleton<InstanceSpecification>(emitter, slot, "owningInstance", &Slot::getOwningInstance);
}

bool emitTemplateBindingScope(YAML::Emitter& emitter, TemplateBinding& templateBinding, __attribute__((unused)) EmitterData& data) {
    return emitSingleton<TemplateableElement>(emitter, templateBinding, "boundElement", &TemplateBinding::getBoundElement);
}

bool emitTemplateParameterScope(YAML::Emitter& emitter, TemplateParameter& templateParameter, __attribute__((unused)) EmitterData& data) {
    return emitSingleton<TemplateSignature>(emitter, templateParameter, "signature", &TemplateParameter::getSignature);
}

bool emitTemplateParameterSubstitutionScope(YAML::Emitter& emitter, TemplateParameterSubstitution& templateParameterSubstitution, __attribute__((unused)) EmitterData& data) {
    return emitSingleton<TemplateBinding>(emitter, templateParameterSubstitution, "templateBinding", &TemplateParameterSubstitution::getTemplateBinding);
}

bool emitTemplateSignatureScope(YAML::Emitter& emitter, TemplateSignature& signature, __attribute__((unused)) EmitterData& data) {
    return emitSingleton<TemplateableElement>(emitter, signature, "template", &TemplateSignature::getTemplate);
}
    
}
