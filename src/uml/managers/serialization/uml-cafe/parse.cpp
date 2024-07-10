#include "uml/managers/serialization/uml-cafe/parse.h"
#include "uml/types/stereotype.h"

using namespace std;

namespace UML {

template <class T, class V = T>
void parseFeatures(YAML::Node node, T& el, ParserData& data, void (*parser)(YAML::Node, V&, ParserData&)) {
    parser(node, el, data);
}

template <class T, class V = T, class ... Funcs>
void parseFeatures(YAML::Node node, T& el, ParserData& data, void (*parser)(YAML::Node, V&, ParserData&), Funcs... funcs) {
    parseFeatures(node, el, data, parser);
    parseFeatures(node, el, data, funcs...);
}

template <class T, class ... Funcs>
UmlPtr<T> createAndParse(YAML::Node node, ParserData& data, Funcs... funcs) {
    if (node.IsMap()) {
        UmlPtr<T> ret;
        if (node["id"]) {
            if (!node["id"].IsScalar()) {
                throw SerializationError("Could not parse id of element because it is not a scalar value " + getLineNumber(node["id"]));
            }
            string idString = node["id"].as<string>();
            if (!isValidID(idString)) {
                throw SerializationError("Could not parse id of element because it is not a valid id. A valid id is a 28 character url safe base64 encoded string " + getLineNumber(node["id"]));
            }
            ID id = ID::fromString(idString);
            if (data.manager->loaded(id)) {
                // update
                data.update = true;
                ret = data.manager->get(id);
            } else {
                ret = data.manager->create<T>();
            }
        } else {
            ret = data.manager->create<T>();
        }
        parseFeatures(node, *ret, data, funcs...);
        return ret;
    } else {
        throw SerializationError("Could not parse data for element of type " + 
                                    Element::elementTypeToString(T::elementType()) + 
                                    " because node specifying it was not a Map node at " + 
                                    getLineNumber(node));
    }
}

template <class T, class V = T>
void parseScope(YAML::Node node, T& el, ParserData& data, bool (*parser)(YAML::Node, V&, ParserData&)) {
    parser(node, el, data);
}

template <class T, class V = T, class ... Funcs>
void parseScope(YAML::Node node, T& el, ParserData& data, bool (*parser)(YAML::Node, V&, ParserData&), Funcs... funcs) {
    if (!parser(node, el, data)) {
        parseScope(node, el, data, funcs...);
    }
} 

template <class T, class U, class S>
void parseSet(YAML::Node node, U& el, ParserData& data, string key, S& (U::*sequenceSignature)()) {
    if (data.update) {
        // remove all elements
        (el.*sequenceSignature)().clear();
    }
    if (node[key]) {
        if (!node[key].IsSequence()) {
            throw SerializationError("Could not parse set " + key + "because it's data was not a sequence! " + getLineNumber(node[key]));
        }
        
        for (size_t i = 0; i < node[key].size(); i++) {
            if (node[key][i].IsMap()) {
                (el.*sequenceSignature)().add(parseNode(node[key][i], data));
            } else if (node[key][i].IsScalar()) {
                ID id = ID::fromString(node[key][i].as<string>());
                if (data.update) {
                    (el.*sequenceSignature)().add(data.manager->get(id)->as<T>());
                } else {
                    (el.*sequenceSignature)().add(id);
                }
            } else {
                throw SerializationError("cannot parse entry of set! " + getLineNumber(node[key][i]));
            }
        }
        
    }
}

template <class T, class U>
bool parseSingleton(YAML::Node node, U& el, ParserData& data, string key, void (U::*idMutator)(ID), void (U::*elMutator)(T&)) {
    if (node[key]) {
        if (node[key].IsMap()) {
            // assuming this is an owned singleton and we are parsing a project serialized in whole
            (el.*elMutator)(parseNode(node[key], data)->as<T>());
        } else if (node[key].IsScalar()) {
            ID id;
             try {
                id = ID::fromString(node[key].as<string>());
            } catch (InvalidUmlID_Exception& exception) {
                throw SerializationError("singleton value must be an ID which is a base64 encoded 28 character string! " + getLineNumber(node[key]));
            }
            if (data.update) {
                try {
                    (el.*elMutator)(data.manager->get(id)->as<T>());
                } catch (std::exception& _e) {
                    (el.*idMutator)(id);
                }
            } else {
                (el.*idMutator)(id);
            }
        } else {
            throw SerializationError("Singleton " + key + " for element type " + Element::elementTypeToString(U::elementType()) + " could not be serialized because it was not a map or scalar " + getLineNumber(node[key]));
        }
        return true;
    } else if (data.update) {
        // set to null ID
        (el.*idMutator)(ID::nullID());
    }
    return false;
}

template <class T>
void parseBoolean(YAML::Node node, T& el, string key, void (T::*mutator)(bool)) {
    if (node[key]) {
        if (!node[key].IsScalar()) {
            throw SerializationError("Could not parse boolean value for property " + key + " for element type " + Element::elementTypeToString(T::elementType()) + " " + getLineNumber(node[key]));
        }
        (el.*mutator)(node[key].as<bool>());
    }
}

template <class T>
void parseInt(YAML::Node node, T& el, string key, void (T::*mutator)(int)) {
    if (node[key]) {
        if (!node[key].IsScalar()) {
            throw SerializationError("Could not parse integer value for property " + key + " for element type " + Element::elementTypeToString(T::elementType()) + " " + getLineNumber(node[key]));
        }
        (el.*mutator)(node[key].as<int>());
    }
}

template <class T>
void parseDouble(YAML::Node node, T& el, string key, void (T::*mutator)(double)) {
    if (node[key]) {
        if (!node[key].IsScalar()) {
            throw SerializationError("Could not parse double value for property " + key + " for element type " + Element::elementTypeToString(T::elementType()) + " " + getLineNumber(node[key]));
        }
        (el.*mutator)(node[key].as<double>());
    }
}

template <class T>
void parseString(YAML::Node node, T& el, string key, void (T::*mutator)(const string&)) {
    if (node[key]) {
        if (!node[key].IsScalar()) {
            throw SerializationError("Could not parse string value for property " + key + " for element type " + Element::elementTypeToString(T::elementType()) + " " + getLineNumber(node[key]));
        }
        (el.*mutator)(node[key].as<string>());
    }
}

string getLineNumber(YAML::Node node) {
    return "line number " + to_string(node.Mark().line);
}

ElementPtr parseNode(YAML::Node node, ParserData& data) {
    ElementPtr ret;
    if (node["Abstraction"]) {
        ret = createAndParse<Abstraction>(node["Abstraction"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseParameterableElementFeatures,
                    parseDependencyFeatures //,
                    // parseAbstractionFeatures
                    );
        parseScope(node, ret->as<Abstraction>(), data,
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["ActionInputPin"]) {
        ret = createAndParse<ActionInputPin>(node["ActionInputPin"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseActivityNodeFeatures,
                    parseTypedElementFeatures,
                    parseObjectNodeFeatures,
                    parseMultiplicityElementFeatures,
                    parsePinFeatures,
                    parseActionInputPinFeatures);
        parseScope(node, ret->as<ActionInputPin>(), data,
                    parseActivityNodeScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["Activity"] && node["Activity"].IsMap()) {
        ret = createAndParse<Activity>(node["Activity"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseNamespaceFeatures,
                    parseParameterableElementFeatures,
                    parseTemplateableElementFeatures,
                    parseClassifierFeatures,
                    parseStructuredClassifierFeatures,
                    parseBehavioredClassifierFeatures,
                    parseClassFeatures,
                    parseBehaviorFeatures,
                    parseActivityFeatures);
        parseScope(node, ret->as<Activity>(), data,
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["ActivityFinalNode"]) {
        ret = createAndParse<ActivityFinalNode>(node["ActivityFinalNode"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseActivityNodeFeatures);
        parseScope(node, ret->as<ActivityFinalNode>(), data,
                    parseActivityNodeScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["ActivityParameterNode"]) {
        ret = createAndParse<ActivityParameterNode>(node["ActivityParameterNode"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseActivityNodeFeatures,
                    parseTypedElementFeatures,
                    parseObjectNodeFeatures,
                    parseActivityParameterNodeFeatures);
        parseScope(node, ret->as<ActivityParameterNode>(), data,
                    parseActivityNodeScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["ActivityPartition"]) {
        ret = createAndParse<ActivityPartition>(node["ActivityPartition"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseActivityPartitionFeatures);
        parseScope(node, ret->as<ActivityPartition>(), data,
                    parseActivityPartitionScope,
                    parseActivityGroupScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["Artifact"]) {
        ret = createAndParse<Artifact>(node["Artifact"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseNamespaceFeatures,
                    parseTemplateableElementFeatures,
                    parseParameterableElementFeatures,
                    parseClassifierFeatures,
                    parseArtifactFeatures);
        parseScope(node, ret->as<Artifact>(), data, 
                    parsePackageableElementScope, 
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["Association"]) {
        ret = createAndParse<Association>(node["Association"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseNamespaceFeatures,
                    parseTemplateableElementFeatures,
                    parseParameterableElementFeatures,
                    parseClassifierFeatures,
                    parseAssociationFeatures);
        parseScope(node, ret->as<Association>(), data,
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["CallBehaviorAction"]) {
        ret = createAndParse<CallBehaviorAction>(node["CallBehaviorAction"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseActivityNodeFeatures,
                    parseExecutableNodeFeatures,
                    parseActionFeatures,
                    parseInvocationActionFeatures,
                    parseCallActionFeatures,
                    parseCallBehaviorActionFeatures);
        parseScope(node, ret->as<CallBehaviorAction>(), data,
                    parseActivityNodeScope,
                    parseNamedElementScope,
                    parseElementScope);
    }else if (node["CentralBufferNode"]) {
        ret = createAndParse<CentralBufferNode>(node["CentralBufferNode"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseActivityNodeFeatures,
                    parseTypedElementFeatures,
                    parseObjectNodeFeatures);
        parseScope(node, ret->as<CentralBufferNode>(), data,
                    parseActivityNodeScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["Class"] && node["Class"].IsMap()) {
        ret = createAndParse<Class>(node["Class"], data,
                    parseElementFeatures,
                    parseNamespaceFeatures,
                    parseNamedElementFeatures,
                    parseTemplateableElementFeatures,
                    parseParameterableElementFeatures,
                    parseClassifierFeatures,
                    parseStructuredClassifierFeatures,
                    parseBehavioredClassifierFeatures,
                    parseClassFeatures);
        parseScope(node, ret->as<Class>(), data, 
                    parsePackageableElementScope, 
                    parseParameterableElementScope,
                    parseNamedElementScope, 
                    parseElementScope);
    } else if (node["ClassifierTemplateParameter"]) {
        ret = createAndParse<ClassifierTemplateParameter>(node["ClassifierTemplateParameter"], data,
                    parseElementFeatures,
                    parseTemplateParameterFeatures,
                    parseClassifierTemplateParameterFeatures);
        parseScope(node, ret->as<ClassifierTemplateParameter>(), data, 
                    parseTemplateParameterScope,
                    parseElementScope);
    } else if (node["Comment"]) {
        ret = createAndParse<Comment>(node["Comment"], data,
                    parseElementFeatures,
                    parseCommentFeatures);
        parseScope(node, ret->as<Comment>(), data, parseElementScope);
    } else if (node["Connector"]) {
        ret = createAndParse<Connector>(node["Connector"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseFeatureFeatures,
                    parseConnectorFeatures);
        parseScope(node, ret->as<Connector>(), data,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["ConnectorEnd"]) {
        ret = createAndParse<ConnectorEnd>(node["ConnectorEnd"], data,
                    parseElementFeatures,
                    parseMultiplicityElementFeatures,
                    parseConnectorEndFeatures);
        parseScope(node, ret->as<ConnectorEnd>(), data,
                    parseElementScope);
    } else if (node["Constraint"]) {
        ret = createAndParse<Constraint>(node["Constraint"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseParameterableElementFeatures,
                    parseConstraintFeatures);
        parseScope(node, ret->as<Constraint>(), data,
                    parseConstraintScope,
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["ControlFlow"]) {
        ret = createAndParse<ControlFlow>(node["ControlFlow"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseActivityEdgeFeatures);
        parseScope(node, ret->as<ControlFlow>(), data, 
                    parseActivityEdgeScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["DataStoreNode"]) {
        ret = createAndParse<DataStoreNode>(node["DataStoreNode"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseActivityNodeFeatures,
                    parseTypedElementFeatures,
                    parseObjectNodeFeatures);
        parseScope(node, ret->as<DataStoreNode>(), data,
                    parseActivityNodeScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["DataType"] && node["DataType"].IsMap()) {
        ret = createAndParse<DataType>(node["DataType"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseNamespaceFeatures,
                    parseTemplateableElementFeatures,
                    parseParameterableElementFeatures,
                    parseClassifierFeatures,
                    parseDataTypeFeatures);
        parseScope(node, ret->as<DataType>(), data, 
                    parsePackageableElementScope, 
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["DecisionNode"]) {
        ret = createAndParse<DecisionNode>(node["DecisionNode"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseActivityNodeFeatures,
                    parseDecisionNodeFeatures);
        parseScope(node, ret->as<DecisionNode>(), data,
                    parseActivityNodeScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["Dependency"]) {
        ret = createAndParse<Dependency>(node["Dependency"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseParameterableElementFeatures,
                    parseDependencyFeatures);
        parseScope(node, ret->as<Dependency>(), data,
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["Deployment"]) {
        ret = createAndParse<Deployment>(node["Deployment"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseParameterableElementFeatures,
                    parseDeploymentFeatures);
        parseScope(node, ret->as<Deployment>(), data, 
                    parseDeploymentScope,
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["ElementImport"]) {
        ret = createAndParse<ElementImport>(node["ElementImport"], data,
                    parseElementFeatures,
                    parseElementImportFeatures);
        parseScope(node, ret->as<ElementImport>(), data, parseElementImportScope, parseElementScope);
    } else if (node["Enumeration"] && node["Enumeration"].IsMap()) {
        ret = createAndParse<Enumeration>(node["Enumeration"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseNamespaceFeatures,
                    parseTemplateableElementFeatures,
                    parseParameterableElementFeatures,
                    parseClassifierFeatures,
                    parseDataTypeFeatures,
                    parseEnumerationFeatures);
        parseScope(node, ret->as<Enumeration>(), data,
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["EnumerationLiteral"]) {
        ret = createAndParse<EnumerationLiteral>(node["EnumerationLiteral"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseParameterableElementFeatures,
                    parseInstanceSpecificationFeatures);
        parseScope(node, ret->as<EnumerationLiteral>(), data, 
                    parseEnumerationLiteralScope, 
                    parsePackageableElementScope, 
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["ExceptionHandler"]) {
        ret = createAndParse<ExceptionHandler>(node["ExceptionHandler"], data,
                    parseElementFeatures,
                    parseExceptionHandlerFeatures);
        parseScope(node, ret->as<ExceptionHandler>(), data, 
                    parseExceptionHandlerScope,
                    parseElementScope);
    } else if (node["Expression"]) {
        ret = createAndParse<Expression>(node["Expression"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseParameterableElementFeatures,
                    parseTypedElementFeatures,
                    parseExpressionFeatures);
        parseScope(node, ret->as<Expression>(), data,
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["Extension"]) {
        ret = createAndParse<Extension>(node["Extension"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseNamespaceFeatures,
                    parseParameterableElementFeatures,
                    parseTemplateableElementFeatures,
                    parseClassifierFeatures,
                    parseExtensionFeatures);
        parseScope(node, ret->as<Extension>(), data,
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["ExtensionEnd"]) {
        ret = createAndParse<ExtensionEnd>(node["ExtensionEnd"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseTypedElementFeatures,
                    parseFeatureFeatures,
                    parseStructuralFeatureFeatures,
                    parseParameterableElementFeatures,
                    parsePropertyFeatures);
        parseScope(node, ret->as<ExtensionEnd>(), data,
                    parsePropertyScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["FinalNode"]) {
        ret = createAndParse<FinalNode>(node["FinalNode"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseActivityNodeFeatures);
        parseScope(node, ret->as<FinalNode>(), data,
                    parseActivityNodeScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["FlowFinalNode"]) {
        ret = createAndParse<FlowFinalNode>(node["FlowFinalNode"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseActivityNodeFeatures);
        parseScope(node, ret->as<FlowFinalNode>(), data,
                    parseActivityNodeScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["ForkNode"]) {
        ret = createAndParse<ForkNode>(node["ForkNode"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseActivityNodeFeatures);
        parseScope(node, ret->as<ForkNode>(), data,
                    parseActivityNodeScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["Generalization"]) {
        ret = createAndParse<Generalization>(node["Generalization"], data, 
                    parseElementFeatures,
                    parseGeneralizationFeatures);
        parseScope(node, ret->as<Generalization>(), data, 
                    parseGeneralizationScope,
                    parseElementScope);
    } else if (node["GeneralizationSet"]) {
        ret = createAndParse<GeneralizationSet>(node["GeneralizationSet"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseParameterableElementFeatures,
                    parseGeneralizationSetFeatures);
        parseScope(node, ret->as<GeneralizationSet>(), data,
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["InitialNode"]) {
        ret = createAndParse<InitialNode>(node["InitialNode"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseActivityNodeFeatures);
        parseScope(node, ret->as<InitialNode>(), data, 
                    parseActivityNodeScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["InputPin"]) {
        ret = createAndParse<InputPin>(node["InputPin"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseActivityNodeFeatures,
                    parseTypedElementFeatures,
                    parseObjectNodeFeatures,
                    parseMultiplicityElementFeatures,
                    parsePinFeatures);
        parseScope(node, ret->as<InputPin>(), data,
                    parseActivityNodeScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["InstanceSpecification"]) {
        ret = createAndParse<InstanceSpecification>(node["InstanceSpecification"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseParameterableElementFeatures,
                    parseDeploymentTargetFeatures,
                    parseInstanceSpecificationFeatures);
        parseScope(node, ret->as<InstanceSpecification>(), data, 
                    parsePackageableElementScope, 
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["InstanceValue"]) {
        ret = createAndParse<InstanceValue>(node["InstanceValue"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseTypedElementFeatures,
                    parseParameterableElementFeatures,
                    parseInstanceValueFeatures);
        parseScope(node, ret->as<InstanceValue>(), data,
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["Interface"] && node["Interface"].IsMap()) {
        ret = createAndParse<Interface>(node["Interface"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseNamespaceFeatures,
                    parseTemplateableElementFeatures,
                    parseParameterableElementFeatures,
                    parseClassifierFeatures,
                    parseInterfaceFeatures);
        parseScope(node, ret->as<Interface>(), data, 
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["InterfaceRealization"]) {
        ret = createAndParse<InterfaceRealization>(node["InterfaceRealization"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseParameterableElementFeatures,
                    parseDependencyFeatures,
                    // TODO abstraction features
                    parseInterfaceRealizationFeatures);
        parseScope(node, ret->as<InterfaceRealization>(), data,
                    parseInterfaceRealizationScope, 
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["JoinNode"]) {
        ret = createAndParse<JoinNode>(node["JoinNode"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseActivityNodeFeatures,
                    parseJoinNodeFeatures);
        parseScope(node, ret->as<JoinNode>(), data,
                    parseActivityNodeScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["LiteralBool"]) {
        ret = createAndParse<LiteralBool>(node["LiteralBool"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseTypedElementFeatures,
                    parseParameterableElementFeatures,
                    parseLiteralBoolFeatures);
        parseScope(node, ret->as<LiteralBool>(), data, 
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["LiteralInt"]) {
        ret = createAndParse<LiteralInt>(node["LiteralInt"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseTypedElementFeatures,
                    parseParameterableElementFeatures,
                    parseLiteralIntegerFeatures);
        parseScope(node, ret->as<LiteralInt>(), data, 
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["LiteralNull"]) {
        ret = createAndParse<LiteralNull>(node["LiteralNull"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseTypedElementFeatures,
                    parseParameterableElementFeatures);
        parseScope(node, ret->as<LiteralNull>(), data,
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["LiteralReal"]) {
        ret = createAndParse<LiteralReal>(node["LiteralReal"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseTypedElementFeatures,
                    parseParameterableElementFeatures,
                    parseLiteralRealFeatures);
        parseScope(node, ret->as<LiteralReal>(), data, 
                    parsePackageableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["LiteralString"]) {
        ret = createAndParse<LiteralString>(node["LiteralString"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseTypedElementFeatures,
                    parseParameterableElementFeatures,
                    parseLiteralStringFeatures);
        parseScope(node, ret->as<LiteralString>(), data, 
                    parsePackageableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["LiteralUnlimitedNatural"]) {
        ret = createAndParse<LiteralUnlimitedNatural>(node["LiteralUnlimitedNatural"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseTypedElementFeatures,
                    parseParameterableElementFeatures,
                    parseLiteralUnlimitedNaturalFeatures);
        parseScope(node, ret->as<LiteralUnlimitedNatural>(), data, 
                    parsePackageableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["MergeNode"]) {
        ret = createAndParse<MergeNode>(node["MergeNode"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseActivityNodeFeatures);
        parseScope(node, ret->as<MergeNode>(), data,
                    parseActivityNodeScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["Manifestation"]) {
        ret = createAndParse<Manifestation>(node["Manifestation"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseParameterableElementFeatures,
                    parseManifestationFeatures);
        parseScope(node, ret->as<Manifestation>(), data, 
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["Model"]) {
        ret = createAndParse<Model>(node["Model"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseNamespaceFeatures,
                    parseParameterableElementFeatures,
                    parseTemplateableElementFeatures,
                    parsePackageFeatures,
                    parseModelFeatures);
        parseScope(node, ret->as<Model>(), data, 
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["ObjectFlow"]) {
        ret = createAndParse<ObjectFlow>(node["ObjectFlow"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseActivityEdgeFeatures,
                    parseObjectFlowFeatures);
        parseScope(node, ret->as<ObjectFlow>(), data,
                    parseActivityEdgeScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["OpaqueAction"]) {
        ret = createAndParse<OpaqueAction>(node["OpaqueAction"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseActivityNodeFeatures,
                    parseExecutableNodeFeatures,
                    parseActionFeatures,
                    parseOpaqueActionFeatures);
        parseScope(node, ret->as<OpaqueAction>(), data,
                    parseActivityNodeScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["OpaqueBehavior"]) {
        ret = createAndParse<OpaqueBehavior>(node["OpaqueBehavior"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseNamespaceFeatures,
                    parseParameterableElementFeatures,
                    parseTemplateableElementFeatures,
                    parseClassifierFeatures,
                    parseClassFeatures,
                    parseBehaviorFeatures,
                    parseOpaqueBehaviorFeatures);
        parseScope(node, ret->as<OpaqueBehavior>(), data, 
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["Operation"] && node["Operation"].IsMap()) {
        ret = createAndParse<Operation>(node["Operation"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseParameterableElementFeatures,
                    parseTemplateableElementFeatures,
                    parseFeatureFeatures,
                    parseBehavioralFeatureFeatures);
        parseScope(node, ret->as<Operation>(), data, 
                    parseOperationScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["OutputPin"]) {
        ret = createAndParse<OutputPin>(node["OutputPin"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseActivityNodeFeatures,
                    parseTypedElementFeatures,
                    parseObjectNodeFeatures,
                    parseMultiplicityElementFeatures,
                    parsePinFeatures);
        parseScope(node, ret->as<OutputPin>(), data,
                    parseActivityNodeScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["Package"]) {
        ret = createAndParse<Package>(node["Package"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseNamespaceFeatures,
                    parseTemplateableElementFeatures,
                    parseParameterableElementFeatures,
                    parsePackageFeatures);
        parseScope(node, ret->as<Package>(), data, 
                    parsePackageableElementScope, 
                    parseParameterableElementScope, 
                    parseNamedElementScope, 
                    parseElementScope);
    } else if (node["PackageImport"]) {
        ret = createAndParse<PackageImport>(node["PackageImport"], data,
                    parseElementFeatures,
                    parsePackageImportFeatures);
        parseScope(node, ret->as<PackageImport>(), data, 
                    parsePackageImportScope,
                    parseElementScope);
    } else if (node["PackageMerge"]) {
        ret = createAndParse<PackageMerge>(node["PackageMerge"], data,
                    parseElementFeatures,
                    parsePackageMergeFeatures);
        parseScope(node, ret->as<PackageMerge>(), data,
                    parsePackageMergeScope,
                    parseElementScope);
    } else if (node["Parameter"]) {
        ret = createAndParse<Parameter>(node["Parameter"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseTypedElementFeatures, // parseConnectableElements
                    parseMultiplicityElementFeatures,
                    parseParameterFeatures);
        parseScope(node, ret->as<Parameter>(), data, 
                    parseParameterScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["ParameterSet"]) {
        ret = createAndParse<ParameterSet>(node["ParameterSet"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseParameterSetFeatures);
        parseScope(node, ret->as<ParameterSet>(), data,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["Port"]) {
        ret = createAndParse<Port>(node["Port"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseFeatureFeatures,
                    parseTypedElementFeatures,
                    parseStructuralFeatureFeatures,
                    parseMultiplicityElementFeatures,
                    parseParameterableElementFeatures,
                    parsePropertyFeatures,
                    parsePortFeatures);
        parseScope(node, ret->as<Port>(), data, 
                    parsePropertyScope, 
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["PrimitiveType"]) {
        ret = createAndParse<PrimitiveType>(node["PrimitiveType"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseNamespaceFeatures,
                    parseTemplateableElementFeatures,
                    parseParameterableElementFeatures,
                    parseClassifierFeatures,
                    parseDataTypeFeatures);
        parseScope(node, ret->as<PrimitiveType>(), data,
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["Profile"]) {
        ret = createAndParse<Profile>(node["Profile"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseParameterableElementFeatures,
                    parseNamespaceFeatures,
                    parseTemplateableElementFeatures,
                    parsePackageFeatures);
        parseScope(node, ret->as<Profile>(), data,
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["ProfileApplication"]) {
        ret = createAndParse<ProfileApplication>(node["ProfileApplication"], data,
                    parseElementFeatures,
                    parseProfileApplicationFeatures);
        parseScope(node, ret->as<ProfileApplication>(), data,
                    parseProfileApplicationScope,
                    parseElementScope);
    } else if (node["Property"]) {
        ret = createAndParse<Property>(node["Property"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseFeatureFeatures,
                    parseTypedElementFeatures,
                    parseStructuralFeatureFeatures,
                    parseMultiplicityElementFeatures,
                    parseParameterableElementFeatures,
                    parseDeploymentTargetFeatures,
                    parsePropertyFeatures);
        parseScope(node, ret->as<Property>(), data, 
                    parsePropertyScope, 
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["Reception"]) {
        ret = createAndParse<Reception>(node["Reception"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseFeatureFeatures,
                    parseBehavioralFeatureFeatures,
                    parseReceptionFeatures);
        parseScope(node, ret->as<Reception>(), data, 
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["Realization"]) {
        ret = createAndParse<Realization>(node["Realization"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseParameterableElementFeatures,
                    parseDependencyFeatures);
                    // TODO abstraction features
        parseScope(node, ret->as<Realization>(), data,
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["RedefinableTemplateSignature"]) {
        ret = createAndParse<RedefinableTemplateSignature>(node["RedefinableTemplateSignature"], data,
                    parseElementFeatures,
                    parseTemplateSignatureFeatures,
                    parseRedefinableTemplateSignatureFeatures);
        parseScope(node, ret->as<RedefinableTemplateSignature>(), data,
                    parseRedefinableTemplateSignatureScope,
                    parseTemplateSignatureScope,
                    parseElementScope);
    } else if (node["Signal"]) {
        ret = createAndParse<Signal>(node["Signal"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseParameterableElementFeatures,
                    parseTemplateableElementFeatures,
                    parseNamespaceFeatures,
                    parseClassifierFeatures,
                    parseSignalFeatures);
        parseScope(node, ret->as<Signal>(), data, 
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["Slot"]) {
        ret = createAndParse<Slot>(node["Slot"], data, 
                    parseElementFeatures,
                    parseSlotFeatures);
        parseScope(node, ret->as<Slot>(), data, 
                    parseSlotScope,
                    parseElementScope);
    } else if (node["Stereotype"]) {
        ret = createAndParse<Stereotype>(node["Stereotype"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseNamespaceFeatures,
                    parseParameterableElementFeatures,
                    parseTemplateableElementFeatures,
                    parseClassifierFeatures,
                    parseStructuredClassifierFeatures,
                    parseBehavioredClassifierFeatures,
                    parseClassFeatures,
                    parseStereotypeFeatures);
        parseScope(node, ret->as<Stereotype>(), data, 
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["TemplateBinding"] && node["TemplateBinding"].IsMap()) {
        ret = createAndParse<TemplateBinding>(node["TemplateBinding"], data,
                    parseElementFeatures,
                    parseTemplateBindingFeatures);
        parseScope(node, ret->as<TemplateBinding>(), data, parseTemplateBindingScope, parseElementScope);
    } else if (node["TemplateParameter"]) {
        ret = createAndParse<TemplateParameter>(node["TemplateParameter"], data,
                    parseElementFeatures,
                    parseTemplateParameterFeatures);
        parseScope(node, ret->as<TemplateParameter>(), data,
                    parseTemplateParameterScope,
                    parseElementScope);
    } else if (node["TemplateParameterSubstitution"]) {
        ret = createAndParse<TemplateParameterSubstitution>(node["TemplateParameterSubstitution"], data,
                        parseElementFeatures,
                        parseTemplateParameterSubstitutionFeatures);
        parseScope(node, ret->as<TemplateParameterSubstitution>(), data, parseTemplateParameterSubstitutionScope, parseElementScope);
    } else if (node["TemplateSignature"]) {
        ret = createAndParse<TemplateSignature>(node["TemplateSignature"], data,
                    parseElementFeatures,
                    parseTemplateSignatureFeatures);
        parseScope(node, ret->as<TemplateSignature>(), data,
                    parseTemplateSignatureScope,
                    parseElementScope);
    } else if (node["Usage"]) {
        ret = createAndParse<Usage>(node["Usage"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseParameterableElementFeatures,
                    parseDependencyFeatures);
        parseScope(node, ret->as<Usage>(), data,
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["ValuePin"]) {
        ret = createAndParse<ValuePin>(node["ValuePin"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseActivityNodeFeatures,
                    parseTypedElementFeatures,
                    parseObjectNodeFeatures,
                    parseMultiplicityElementFeatures,
                    parsePinFeatures,
                    parseValuePinFeatures);
        parseScope(node, ret->as<ValuePin>(), data,
                    parseActivityNodeScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else {
        throw SerializationError("Could not identify an element type to parse in data provided for parsing " + getLineNumber(node));
    }
    return ret;
}

ElementType elementTypeFromString(string eType) {
    if (eType.compare("ABSTRACTION") == 0) {
        return ElementType::ABSTRACTION;
    } else if (eType.compare("ACTION") == 0) {
        return ElementType::ACTION;
    } else if (eType.compare("ACTION_INPUT_PIN") == 0) {
        return ElementType::ACTION_INPUT_PIN;
    } else if (eType.compare("ACTIVITY") == 0) {
        return ElementType::ACTIVITY;
    } else if (eType.compare("ACTIVITY_EDGE") == 0) {
        return ElementType::ACTIVITY_EDGE;
    } else if (eType.compare("ACTIVITY_FINAL_NODE") == 0) {
        return ElementType::ACTIVITY_FINAL_NODE;
    } else if (eType.compare("ACTIVITY_GROUP") == 0) {
        return ElementType::ACTIVITY_GROUP;
    } else if (eType.compare("ACTIVITY_NODE") == 0) {
        return ElementType::ACTIVITY_NODE;
    } else if (eType.compare("ACTIVITY_PARAMETER_NODE") == 0) {
        return ElementType::ACTIVITY_PARAMETER_NODE;
    } else if (eType.compare("ACTIVITY_PARTITION") == 0) {
        return ElementType::ACTIVITY_PARTITION;
    } else if (eType.compare("ARTIFACT") == 0) {
        return ElementType::ARTIFACT;
    } else if (eType.compare("ASSOCIATION") == 0) {
        return ElementType::ASSOCIATION;
    } else if (eType.compare("BEHAVIOR") == 0) {
        return ElementType::BEHAVIOR;
    } else if (eType.compare("BEHAVIORAL_FEATURE") == 0) {
        return ElementType::BEHAVIORAL_FEATURE;
    } else if (eType.compare("BEHAVIORED_CLASSIFIER") == 0 ) {
        return ElementType::BEHAVIORED_CLASSIFIER;
    } else if (eType.compare("CALL_ACTION") == 0) {
        return ElementType::CALL_ACTION;
    } else if (eType.compare("CALL_BEHAVIOR_ACTION") == 0) {
        return ElementType::CALL_BEHAVIOR_ACTION;
    } else if (eType.compare("CENTRAL_BUFFER_NODE") == 0) {
        return ElementType::CENTRAL_BUFFER_NODE;
    } else if (eType.compare("CLASS") == 0) {
        return ElementType::CLASS;
    } else if (eType.compare("CLASSIFIER") == 0) {
        return ElementType::CLASSIFIER;
    } else if (eType.compare("COMMENT") == 0) {
        return ElementType::COMMENT;
    } else if (eType.compare("CLASSIFIER_TEMPLATE_PARAMETER") == 0) {
        return ElementType::CLASSIFIER_TEMPLATE_PARAMETER;
    } else if (eType.compare("CONNECTOR") == 0) {
        return ElementType::CONNECTOR;
    } else if (eType.compare("CONNECTOR_END") == 0) {
        return ElementType::CONNECTOR_END;
    } else if (eType.compare("CONNECTABLE_ELEMENT") == 0) {
        return ElementType::CONNECTABLE_ELEMENT;
    } else if (eType.compare("CONSTRAINT") == 0) {
        return ElementType::CONSTRAINT;
    } else if (eType.compare("CONTROL_FLOW") == 0) {
        return ElementType::CONTROL_FLOW;
    } else if (eType.compare("CREATE_OBJECT_ACTION") == 0) {
        return ElementType::CREATE_OBJECT_ACTION;
    } else if (eType.compare("DATA_STORE_NODE") == 0) {
        return ElementType::DATA_STORE_NODE;
    } else if (eType.compare("DATA_TYPE") == 0) {
        return ElementType::DATA_TYPE;
    } else if (eType.compare("DECISION_NODE") == 0) {
        return ElementType::DECISION_NODE;
    } else if (eType.compare("DEPENDENCY") == 0) {
        return ElementType::DEPENDENCY;
    } else if (eType.compare("DEPLOYED_ARTIFACT") == 0) {
        return ElementType::DEPLOYED_ARTIFACT;
    } else if (eType.compare("DEPLOYMENT") == 0) {
        return ElementType::DEPLOYMENT;
    } else if (eType.compare("DEPLOYMENT_TARGET") == 0) {
        return ElementType::DEPLOYMENT_TARGET;
    } else if (eType.compare("DIRECTED_RELATIONSHIP") == 0) {
        return ElementType::DIRECTED_RELATIONSHIP;
    } else if (eType.compare("ELEMENT") == 0) {
        return ElementType::ELEMENT;
    } else if (eType.compare("ELEMENT_IMPORT") == 0) {
        return ElementType::ELEMENT_IMPORT;
    } else if (eType.compare("ENUMERATION") == 0) {
        return ElementType::ENUMERATION;
    } else if (eType.compare("ENUMERATION_LITERAL") == 0) {
        return ElementType::ENUMERATION_LITERAL;
    } else if (eType.compare("EXCEPTION_HANDLER") == 0) {
        return ElementType::EXCEPTION_HANDLER;
    } else if (eType.compare("EXECUTABLE_NODE") == 0) {
        return ElementType::EXECUTABLE_NODE;
    } else if (eType.compare("EXPRESSION") == 0) {
        return ElementType::EXPRESSION;
    } else if (eType.compare("EXTENSION") == 0) {
        return ElementType::EXTENSION;
    } else if (eType.compare("EXTENSION_END") == 0) {
        return ElementType::EXTENSION_END;
    } else if (eType.compare("FEATURE") == 0) {
        return ElementType::FEATURE;
    } else if (eType.compare("FINAL_NODE") == 0) {
        return ElementType::FINAL_NODE;
    } else if (eType.compare("FORK_NODE") == 0) {
        return ElementType::FORK_NODE;
    } else if (eType.compare("GENERALIZATION") == 0) {
        return ElementType::GENERALIZATION;
    } else if (eType.compare("GENERALIZATION_SET") == 0) {
        return ElementType::GENERALIZATION_SET;
    } else if (eType.compare("INITITAL_NODE") == 0) {
        return ElementType::INITIAL_NODE;
    } else if (eType.compare("INPUT_PIN") == 0) {
        return ElementType::INPUT_PIN;
    } else if (eType.compare("INSTANCE_SPECIFICATION") == 0) {
        return ElementType::INSTANCE_SPECIFICATION;
    } else if (eType.compare("INSTANCE_VALUE") == 0) {
        return ElementType::INSTANCE_VALUE;
    } else if (eType.compare("INTERFACE") == 0) {
        return ElementType::INTERFACE_UML;
    } else if (eType.compare("INTERFACE_REALIZATION") == 0) {
        return ElementType::INTERFACE_REALIZATION;
    } else if (eType.compare("INTERRUPTIBLE_ACTIVITY_REGION") == 0) {
        return ElementType::INTERRUPTIBLE_ACTIVITY_REGION;
    } else if (eType.compare("INVOCATION_ACTION") == 0) {
        return ElementType::INVOCATION_ACTION;
    } else if (eType.compare("JOIN_NODE") == 0) {
        return ElementType::JOIN_NODE;
    } else if (eType.compare("LITERAL_BOOL") == 0) {
        return ElementType::LITERAL_BOOL;
    } else if (eType.compare("LITERAL_INT") == 0) {
        return ElementType::LITERAL_INT;
    } else if (eType.compare("LITERAL_NULL") == 0) {
        return ElementType::LITERAL_NULL;
    } else if (eType.compare("LITERAL_REAL") == 0) {
        return ElementType::LITERAL_REAL;
    } else if (eType.compare("LITERAL_SPECIFICATION") == 0) {
        return ElementType::LITERAL_SPECIFICATION;
    } else if (eType.compare("LITERAL_STRING") == 0) {
        return ElementType::LITERAL_STRING;
    } else if (eType.compare("LITERAL_UNLIMITED_NATURAL") == 0) {
        return ElementType::LITERAL_UNLIMITED_NATURAL;
    } else if (eType.compare("MANIFESTATION") == 0) {
        return ElementType::MANIFESTATION;
    } else if (eType.compare("MERGE_NODE") == 0) {
        return ElementType::MERGE_NODE;
    } else if (eType.compare("MODEL") == 0) {
        return ElementType::MODEL;
    } else if (eType.compare("MULTIPLICITY_ELEMENT") == 0) {
        return ElementType::MULTIPLICITY_ELEMENT;
    } else if (eType.compare("NAMED_ELEMENT") == 0) {
        return ElementType::NAMED_ELEMENT;
    } else if (eType.compare("NAMESPACE") == 0) {
        return ElementType::NAMESPACE;
    } else if (eType.compare("OBJECT_FLOW") == 0) {
        return ElementType::OBJECT_FLOW;
    } else if (eType.compare("OBJECT_NODE") == 0) {
        return ElementType::OBJECT_NODE;
    } else if (eType.compare("OPAQUE_ACTION") == 0) {
        return ElementType::OPAQUE_ACTION;
    } else if (eType.compare("OPAQUE_BEHAVIOR") == 0) {
        return ElementType::OPAQUE_BEHAVIOR;
    } else if (eType.compare("OPERATION") == 0) {
        return ElementType::OPERATION;
    } else if (eType.compare("OUTPUT_PIN") == 0) {
        return ElementType::OUTPUT_PIN;
    } else if (eType.compare("PACKAGE") == 0) {
        return ElementType::PACKAGE;
    } else if (eType.compare("PACKAGEABLE_ELEMENT") == 0) {
        return ElementType::PACKAGEABLE_ELEMENT;
    } else if (eType.compare("PACKAGE_IMPORT") == 0) {
        return ElementType::PACKAGE_IMPORT;
    } else if (eType.compare("PACKAGE_MERGE") == 0) {
        return ElementType::PACKAGE_MERGE;
    } else if (eType.compare("PARAMETER") == 0) {
        return ElementType::PARAMETER;
    } else if (eType.compare("PARAMETERABLE_ELEMENT") == 0) {
        return ElementType::PARAMETERABLE_ELEMENT;
    } else if (eType.compare("PARAMETER_SET") == 0) {
        return ElementType::PARAMETER_SET;
    } else if (eType.compare("PIN") == 0) {
        return ElementType::PIN;
    } else if (eType.compare("PORT") == 0) {
        return ElementType::PORT;
    } else if (eType.compare("PRIMITIVE_TYPE") == 0) {
        return ElementType::PRIMITIVE_TYPE;
    } else if (eType.compare("PROFILE") == 0) {
        return ElementType::PROFILE;
    } else if (eType.compare("PROFILE_APPLICATION") == 0) {
        return ElementType::PROFILE_APPLICATION;
    } else if (eType.compare("PROPERTY") == 0) {
        return ElementType::PROPERTY;
    } else if (eType.compare("REALIZATION") == 0) {
        return ElementType::REALIZATION;
    } else if (eType.compare("RECEPTION") == 0) {
        return ElementType::RECEPTION;
    } else if (eType.compare("REDEFINABLE_ELEMENT") == 0) {
        return ElementType::REDEFINABLE_ELEMENT;
    } else if (eType.compare("REDEFINABLE_TEMPLATE_SIGNATURE") == 0) {
        return ElementType::REDEFINABLE_TEMPLATE_SIGNATURE;
    } else if (eType.compare("RELATIONSHIP") == 0) {
        return ElementType::RELATIONSHIP;
    } else if (eType.compare("SIGNAL") == 0) {
        return ElementType::SIGNAL;
    } else if (eType.compare("SLOT") == 0) {
        return ElementType::SLOT;
    } else if (eType.compare("STEREOTYPE") == 0) {
        return ElementType::STEREOTYPE;
    } else if (eType.compare("STRUCTURAL_FEATURE") == 0) {
        return ElementType::STRUCTURAL_FEATURE;
    } else if (eType.compare("STRUCTURED_CLASSIFIER") == 0) {
        return ElementType::STRUCTURED_CLASSIFIER;
    } else if (eType.compare("TEMPLATEABLE_ELEMENT") == 0) {
        return ElementType::TEMPLATEABLE_ELEMENT;
    } else if (eType.compare("TEMPLATE_BINDING") == 0) {
        return ElementType::TEMPLATE_BINDING;
    } else if (eType.compare("TEMPLATE_PARAMETER") == 0) {
        return ElementType::TEMPLATE_PARAMETER;
    } else if (eType.compare("TEMPLATE_PARAMETER_SUBSTITUTION") == 0) {
        return ElementType::TEMPLATE_PARAMETER_SUBSTITUTION;
    } else if (eType.compare("TEMPLATE_SIGNATURE") == 0) {
        return ElementType::TEMPLATE_SIGNATURE;
    } else if (eType.compare("TYPE") == 0) {
        return ElementType::TYPE;
    } else if (eType.compare("TYPED_ELEMENT") == 0) {
        return ElementType::TYPED_ELEMENT;
    } else if (eType.compare("USAGE") == 0) {
        return ElementType::USAGE;
    } else if (eType.compare("VALUE_PIN") == 0) {
        return ElementType::VALUE_PIN;
    } else if (eType.compare("VALUE_SPECIFICATION") == 0) {
        return ElementType::VALUE_SPECIFICATION;
    }
    throw SerializationError("Could not identify element type by keyword: " + eType + '!');
}

void parseActionFeatures(YAML::Node node, Action& action, ParserData& data) {
    parseSet<Constraint>(node, action, data, "localPreconditions", &Action::getLocalPreconditions);
    parseSet<Constraint>(node, action, data, "localPostconditions", &Action::getLocalPostconditions);
}

void parseActionInputPinFeatures(YAML::Node node, ActionInputPin& actionInputPin, ParserData& data) {
    parseSingleton(node, actionInputPin, data, "fromAction", &ActionInputPin::setFromAction, &ActionInputPin::setFromAction);
}

void parseActivityFeatures(YAML::Node node, Activity& activity, ParserData& data) {
    parseSet<ActivityNode>(node, activity, data, "nodes", &Activity::getNodes);
    parseSet<ActivityEdge>(node, activity, data, "edges", &Activity::getEdges);
    parseSet<ActivityPartition>(node, activity, data, "partitions", &Activity::getPartitions);
}

void parseActivityEdgeFeatures(YAML::Node node, ActivityEdge& activityEdge, ParserData& data) {
    parseSingleton(node, activityEdge, data, "target", &ActivityEdge::setTarget, &ActivityEdge::setTarget);
    parseSingleton(node, activityEdge, data, "source", &ActivityEdge::setSource, &ActivityEdge::setSource);
    parseSingleton(node, activityEdge, data, "guard", &ActivityEdge::setGuard, &ActivityEdge::setGuard);
    parseSingleton(node, activityEdge, data, "weight", &ActivityEdge::setWeight, &ActivityEdge::setWeight);
    parseSet<ActivityPartition>(node, activityEdge, data, "inPartitions", &ActivityEdge::getInPartitions);
}

void parseActivityNodeFeatures(YAML::Node node, ActivityNode& activityNode, ParserData& data) {
    parseSet<ActivityEdge>(node, activityNode, data, "incoming", &ActivityNode::getIncoming);
    parseSet<ActivityEdge>(node, activityNode, data, "outgoing", &ActivityNode::getOutgoing);
    // parseSet<ActivityNode>(node, activityNode, data, "redefinedNodes", &Activity::getRedefinedNodes);
    parseSet<ActivityPartition>(node, activityNode, data, "inPartitions", &ActivityNode::getInPartitions);
}

void parseActivityParameterNodeFeatures(YAML::Node node, ActivityParameterNode& activityParameterNode, ParserData& data) {
    parseSingleton(node, activityParameterNode, data, "parameter", &ActivityParameterNode::setParameter, &ActivityParameterNode::setParameter);
}

void parseActivityPartitionFeatures(YAML::Node node, ActivityPartition& activityPartition, ParserData& data) {
    parseSet<ActivityNode>(node, activityPartition, data, "nodes", &ActivityPartition::getNodes);
    parseSet<ActivityEdge>(node, activityPartition, data, "edges", &ActivityPartition::getEdges);
    parseSet<ActivityPartition>(node, activityPartition, data, "subPartitions", &ActivityPartition::getSubPartitions);
    parseSingleton(node, activityPartition, data, "represents", &ActivityPartition::setRepresents, &ActivityPartition::setRepresents);
}

void parseArtifactFeatures(YAML::Node node, Artifact& artifact, ParserData& data) {
    parseSet<Property>(node, artifact, data, "ownedAttributes", &Artifact::getOwnedAttributes);
    parseSet<Operation>(node, artifact, data, "ownedOperations", &Artifact::getOwnedOperations);
    parseSet<Artifact>(node, artifact, data, "nestedArtifacts", &Artifact::getNestedArtifacts);
    parseSet<Manifestation>(node, artifact, data, "manifestations", &Artifact::getManifestations);
}

void parseAssociationFeatures(YAML::Node node, Association& association, ParserData& data) {
    parseSet<Property>(node, association, data, "memberEnds", &Association::getMemberEnds);
    parseSet<Property>(node, association, data, "ownedEnds", &Association::getOwnedEnds);
    parseSet<Property>(node, association, data, "navigableOwnedEnds", &Association::getNavigableOwnedEnds);
}

void parseBehaviorFeatures(YAML::Node node, Behavior& behavior, ParserData& data) {
    parseSingleton<BehavioralFeature>(node, behavior, data, "specification", &Behavior::setSpecification, &Behavior::setSpecification);
    parseSet<Parameter>(node, behavior, data, "ownedParameters", &Behavior::getOwnedParameters);
}

void parseBehavioralFeatureFeatures(YAML::Node node, BehavioralFeature& behavioralFeature, ParserData& data) {
    if (node["concurrency"]) {
        string concurrencyValue = node["concurrency"].as<string>();
        if (concurrencyValue == "sequential") {
            behavioralFeature.setConcurrency(CallConcurrencyKind::Sequential);
        } else if (concurrencyValue == "guarded") {
            behavioralFeature.setConcurrency(CallConcurrencyKind::Guarded);
        } else if (concurrencyValue == "concurrent") {
            behavioralFeature.setConcurrency(CallConcurrencyKind::Concurrent);
        } else {
            throw SerializationError("Could not parse concurrency, must be sequential, guarded or concurrent! " + getLineNumber(node["concurrency"]));
        }
    }
    parseSet<Type>(node, behavioralFeature, data, "raisedExceptions", &BehavioralFeature::getRaisedExceptions);
    parseSet<Behavior>(node, behavioralFeature, data, "methods", &BehavioralFeature::getMethods);
    parseSet<Parameter>(node, behavioralFeature, data, "ownedParameters", &BehavioralFeature::getOwnedParameters);
    parseSet<ParameterSet>(node, behavioralFeature, data, "ownedParameterSets", &BehavioralFeature::getOwnedParameterSets);
}

void parseBehavioredClassifierFeatures(YAML::Node node, BehavioredClassifier& behavioredClassifier, ParserData& data) {
    parseSingleton<Behavior>(node, behavioredClassifier, data, "classifierBehavior", &BehavioredClassifier::setClassifierBehavior, &BehavioredClassifier::setClassifierBehavior);
    parseSet<Behavior>(node, behavioredClassifier, data, "ownedBehaviors", &BehavioredClassifier::getOwnedBehaviors);
    parseSet<InterfaceRealization>(node, behavioredClassifier, data, "interfaceRealizations", &BehavioredClassifier::getInterfaceRealizations);
}

void parseCallActionFeatures(YAML::Node node, CallAction& callAction, ParserData& data) {
    parseSet<OutputPin>(node, callAction, data, "results", &CallAction::getResults);
}

void parseCallBehaviorActionFeatures(YAML::Node node, CallBehaviorAction& callBehaviorAction, ParserData& data) {
    parseSingleton(node, callBehaviorAction, data, "behavior", &CallBehaviorAction::setBehavior, &CallBehaviorAction::setBehavior);
}

void parseClassFeatures(YAML::Node node, Class& clazz, ParserData& data) {
    parseSet<Classifier>(node, clazz, data, "nestedClassifiers", &Class::getNestedClassifiers);
    parseSet<Property>(node, clazz, data, "ownedAttributes", &Class::getOwnedAttributes);
    parseSet<Operation>(node, clazz, data, "ownedOperations", &Class::getOwnedOperations);
    parseSet<Reception>(node, clazz, data, "ownedReceptions", &Class::getOwnedReceptions);
}

void parseClassifierFeatures(YAML::Node node, Classifier& classifier, ParserData& data) {
    parseSet<Generalization>(node, classifier, data, "generalizations", &Classifier::getGeneralizations);
    parseSet<GeneralizationSet>(node, classifier, data, "powertypeExtent", &Classifier::getPowerTypeExtent);
}

void parseClassifierTemplateParameterFeatures(YAML::Node node, ClassifierTemplateParameter& classifierTemplateParameter, ParserData& data) {
    parseSet<Classifier>(node, classifierTemplateParameter, data, "constrainingClassifiers", &ClassifierTemplateParameter::getConstrainingClassifiers);
}

void parseCommentFeatures(YAML::Node node, Comment& comment, ParserData& data) {
    parseSet<Element>(node, comment, data, "annotatedElements", &Comment::getAnnotatedElements);
    parseString(node, comment, "body", &Comment::setBody);
}

void parseConnectorFeatures(YAML::Node node, Connector& connector, ParserData& data) {
    parseSet<ConnectorEnd>(node, connector, data, "ends", &Connector::getEnds);
    parseSet<Behavior>(node, connector, data, "contracts", &Connector::getContracts);
    parseSingleton<Association>(node, connector, data, "type", &Connector::setType, &Connector::setType);
}

void parseConnectorEndFeatures(YAML::Node node, ConnectorEnd& connectorEnd, ParserData& data) {
    parseSingleton<ConnectableElement>(node, connectorEnd, data, "role", &ConnectorEnd::setRole, &ConnectorEnd::setRole);
}

void parseConstraintFeatures(YAML::Node node, Constraint& constraint, ParserData& data) {
    parseSet<Element>(node, constraint, data, "constrainedElements", &Constraint::getConstrainedElements);
    parseSingleton<ValueSpecification>(node, constraint, data, "specification", &Constraint::setSpecification, &Constraint::setSpecification);
}

void parseDataTypeFeatures(YAML::Node node, DataType& dataType, ParserData& data) {
    parseSet<Property>(node, dataType, data, "ownedAttributes", &DataType::getOwnedAttributes);
    parseSet<Operation>(node, dataType, data, "ownedOperations", &DataType::getOwnedOperations);
}

void parseDecisionNodeFeatures(YAML::Node node, DecisionNode& decisionNode, ParserData& data) {
    parseSingleton(node, decisionNode, data, "decisionInput", &DecisionNode::setDecisionInput, &DecisionNode::setDecisionInput);
    parseSingleton(node, decisionNode, data, "decisionInputFlow", &DecisionNode::setDecisionInputFlow, &DecisionNode::setDecisionInputFlow);
}

void parseDependencyFeatures(YAML::Node node, Dependency& dependency, ParserData& data) {
    parseSet<NamedElement>(node, dependency, data, "suppliers", &Dependency::getSuppliers);
    parseSet<NamedElement>(node, dependency, data, "clients", &Dependency::getClients);
}

void parseDeploymentTargetFeatures(YAML::Node node, DeploymentTarget& deploymentTarget, ParserData& data) {
    parseSet<Deployment>(node, deploymentTarget, data, "deployments", &DeploymentTarget::getDeployments);
}

void parseDeploymentFeatures(YAML::Node node, Deployment& deployment, ParserData& data) {
    parseSet<DeployedArtifact>(node, deployment, data, "deployedArtifacts", &Deployment::getDeployedArtifacts);
}

void parseElementFeatures(YAML::Node node, Element& el, ParserData& data) {
    if (node["id"]) {
        if (!node["id"].IsScalar()) {
            throw SerializationError("Could not parse id of element because it is not a scalar value " + getLineNumber(node["id"]));
        }
        string idString = node["id"].as<string>();
        if (!isValidID(idString)) {
            throw SerializationError("Could not parse id of element because it is not a valid id. A valid id is a 28 character url safe base64 encoded string " + getLineNumber(node["id"]));
        }
        if (!data.update) {
            el.setID(idString);   
        }
    }
    parseSet<Comment>(node, el, data, "ownedComments", &Element::getOwnedComments);
    parseSet<InstanceSpecification>(node, el, data, "appliedStereotypes", &Element::getAppliedStereotypes);
}

template <class T>
void parseVisibilty(YAML::Node node, T& elWithVisibility) {
    if (node["visibility"]) {
        if (!node["visibility"].IsScalar()) {
            throw SerializationError("could not parse visibility, node must be a scalar!" + getLineNumber(node["visibility"]));
        }
        string visibilityValue = node["visibility"].as<string>();
        if (visibilityValue == "package") {
            elWithVisibility.setVisibility(VisibilityKind::PACKAGE);
        } else if (visibilityValue == "private") {
            elWithVisibility.setVisibility(VisibilityKind::PRIVATE);
        } else if (visibilityValue == "protected") {
            elWithVisibility.setVisibility(VisibilityKind::PROTECTED);
        }
    }
}

void parseElementImportFeatures(YAML::Node node, ElementImport& elementImport, ParserData& data) {
    parseSingleton<PackageableElement>(node, elementImport, data, "importedElement", &ElementImport::setImportedElement, &ElementImport::setImportedElement);
    parseString(node, elementImport, "alias", &ElementImport::setAlias);
    parseVisibilty(node, elementImport);
}

void parseEnumerationFeatures(YAML::Node node, Enumeration& enumeration, ParserData& data) {
    parseSet<EnumerationLiteral>(node, enumeration, data, "ownedLiterals", &Enumeration::getOwnedLiterals);
}

void parseExceptionHandlerFeatures(YAML::Node node, ExceptionHandler& exceptionHandler, ParserData& data) {
    parseSingleton(node, exceptionHandler, data, "handlerBody", &ExceptionHandler::setHandlerBody, &ExceptionHandler::setHandlerBody);
    parseSingleton(node, exceptionHandler, data, "exceptionInput", &ExceptionHandler::setExceptionInput, &ExceptionHandler::setExceptionInput);
    parseSet<Classifier>(node, exceptionHandler, data, "exceptionTypes", &ExceptionHandler::getExceptionTypes);
}

void parseExecutableNodeFeatures(YAML::Node node, ExecutableNode& executableNode, ParserData& data) {
    parseSet<ExceptionHandler>(node, executableNode, data, "handlers", &ExecutableNode::getHandlers);
}

void parseExpressionFeatures(YAML::Node node, Expression& expression, ParserData& data) {
    parseSet<ValueSpecification>(node, expression, data, "operands", &Expression::getOperands);
    parseString(node, expression, "symbol", &Expression::setSymbol);
}

void parseExtensionFeatures(YAML::Node node, Extension& extension, ParserData& data) {
    parseSet<Property, Association>(node, extension, data, "memberEnds", &Association::getMemberEnds);
    parseSingleton<ExtensionEnd>(node, extension, data, "ownedEnd", &Extension::setOwnedEnd, &Extension::setOwnedEnd);
    // if (node["metaClass"]) { // this implementation might change
    //     if (!node["metaClass"].IsScalar()) {
    //         throw SerializationError("Expected metaClass to be a scalar value " + getLineNumber(node["metaClass"]));
    //     }
    //     extension.setMetaClass(elementTypeFromString(node["metaClass"].as<string>()));
    // }
}

void parseFeatureFeatures(YAML::Node node, Feature& feature, __attribute__((unused)) ParserData& data) {
    parseBoolean(node, feature, "isStatic", &Feature::setStatic);
}

void parseGeneralizationFeatures(YAML::Node node, Generalization& generalization, ParserData& data) {
    parseSingleton<Classifier>(node, generalization, data, "general", &Generalization::setGeneral, &Generalization::setGeneral);
    parseSet<GeneralizationSet>(node, generalization, data, "generalizationSets", &Generalization::getGeneralizationSets);
}

void parseGeneralizationSetFeatures(YAML::Node node, GeneralizationSet& generalizationSet, ParserData& data) {
    parseSingleton<Classifier>(node, generalizationSet, data, "powertype", &GeneralizationSet::setPowerType, &GeneralizationSet::setPowerType);
    parseSet<Generalization>(node, generalizationSet, data, "generalizations", &GeneralizationSet::getGeneralizations);
    parseBoolean(node, generalizationSet, "isCovering", &GeneralizationSet::setCovering);
    parseBoolean(node, generalizationSet, "isDisjoint", &GeneralizationSet::setDisjoint);
}

void parseInstanceSpecificationFeatures(YAML::Node node, InstanceSpecification& inst, ParserData& data) {
    parseSet<Slot>(node, inst, data, "slots", &InstanceSpecification::getSlots);
    parseSingleton<ValueSpecification>(node, inst, data, "specification", &InstanceSpecification::setSpecification, &InstanceSpecification::setSpecification);
    parseSet<Classifier>(node, inst, data, "classifiers", &InstanceSpecification::getClassifiers);
}

void parseInstanceValueFeatures(YAML::Node node, InstanceValue& instanceValue, ParserData& data) {
    parseSingleton<InstanceSpecification>(node, instanceValue, data, "instance", &InstanceValue::setInstance, &InstanceValue::setInstance);
}

void parseInterfaceFeatures(YAML::Node node, Interface& interface, ParserData& data) {
    parseSet<Property>(node, interface, data, "ownedAttributes", &Interface::getOwnedAttributes);
    parseSet<Operation>(node, interface, data, "ownedOperations", &Interface::getOwnedOperations);
    // parseSet<Reception>(node, interface, data, "ownedReceptions", &Interface::getOwnedReceptions);
    parseSet<Classifier>(node, interface, data, "nestedClassifiers", &Interface::getNestedClassifiers);
    // parseSet<Interface>(node, interface, data, "redefinedInterfaces", &Interface::getRedefinedInterfaces);
}

void parseInterfaceRealizationFeatures(YAML::Node node, InterfaceRealization& interfaceRealization, ParserData& data) {
    parseSingleton<Interface>(node, interfaceRealization, data, "contract", &InterfaceRealization::setContract, &InterfaceRealization::setContract);
}

void parseInvocationActionFeatures(YAML::Node node, InvocationAction& invocationAction, ParserData& data) {
    parseSet<InputPin>(node, invocationAction, data, "arguments", &InvocationAction::getArguments);
    parseSingleton(node, invocationAction, data, "onPort", &InvocationAction::setOnPort, &InvocationAction::setOnPort);
}

void parseJoinNodeFeatures(YAML::Node node, JoinNode& joinNode, ParserData& data) {
    parseSingleton(node, joinNode, data, "joinSpec", &JoinNode::setJoinSpec, &JoinNode::setJoinSpec);
}

void parseLiteralBoolFeatures(YAML::Node node, LiteralBool& literalBool, __attribute__((unused)) ParserData& data) {
    parseBoolean(node, literalBool, "value", &LiteralBool::setValue);
}

void parseLiteralIntegerFeatures(YAML::Node node, LiteralInt& literalInt, __attribute__((unused)) ParserData& data) {
    parseInt(node, literalInt, "value", &LiteralInt::setValue);
}

void parseLiteralRealFeatures(YAML::Node node, LiteralReal& literalReal, __attribute__((unused)) ParserData& data) {
    parseDouble(node, literalReal, "value", &LiteralReal::setValue);
}

void parseLiteralStringFeatures(YAML::Node node, LiteralString& literalString, __attribute__((unused)) ParserData& data) {
    parseString(node, literalString, "value", &LiteralString::setValue);
}

void parseLiteralUnlimitedNaturalFeatures(YAML::Node node, LiteralUnlimitedNatural& literalUnlimitedNatural, __attribute__((unused)) ParserData& data) {
    // special parsing
    if (node["value"]) {
        if (!node["value"].IsScalar()) {
            throw SerializationError("could not parse unlimited natural value from property " + getLineNumber(node["value"]));
        }
        string nodeStringValue = node["value"].as<string>();
        if (nodeStringValue == "*") {
            literalUnlimitedNatural.setInfinite();
        } else {
            literalUnlimitedNatural.setNumberValue(node["value"].as<unsigned long>());
        }
    }
}

void parseManifestationFeatures(YAML::Node node, Manifestation& manifestation, ParserData& data) {
    parseSingleton(node, manifestation, data, "utilizedElement", &Manifestation::setUtilizedElement, &Manifestation::setUtilizedElement);
}

void parseModelFeatures(YAML::Node node, Model& model, __attribute__((unused)) ParserData& data) {
    parseString(node, model, "viewpoint", &Model::setViewpoint);
}

void parseMultiplicityElementFeatures(YAML::Node node, MultiplicityElement& multiplicityElement, ParserData& data) {
    parseBoolean(node, multiplicityElement, "isOrdered", &MultiplicityElement::setIsOrdered);
    parseBoolean(node, multiplicityElement, "isUnique", &MultiplicityElement::setIsUnique);
    parseSingleton<ValueSpecification>(node, multiplicityElement, data, "lowerValue", &MultiplicityElement::setLowerValue, &MultiplicityElement::setLowerValue);
    parseInt(node, multiplicityElement, "lower", &MultiplicityElement::setLower);
    parseSingleton<ValueSpecification>(node, multiplicityElement, data, "upperValue", &MultiplicityElement::setUpperValue, &MultiplicityElement::setUpperValue);
    parseInt(node, multiplicityElement, "upper", &MultiplicityElement::setUpper);
}

void parseNamedElementFeatures(YAML::Node node, NamedElement& el, ParserData& data) {
    parseString(node, el, "name", &NamedElement::setName);
    parseVisibilty(node, el);
    parseSet<Dependency>(node, el, data, "clientDependencies", &NamedElement::getClientDependencies);
}

void parseNamespaceFeatures(YAML::Node node, Namespace& nmspc, ParserData& data) {
    parseSet<ElementImport>(node, nmspc, data, "elementImports", &Namespace::getElementImports);
    parseSet<PackageImport>(node, nmspc, data, "packageImports", &Namespace::getPackageImports);
    parseSet<Constraint>(node, nmspc, data, "ownedRules", &Namespace::getOwnedRules);
}

void parseObjectFlowFeatures(YAML::Node node, ObjectFlow& objectFlow, ParserData& data) {
    parseSingleton(node, objectFlow, data, "transformation", &ObjectFlow::setTransformation, &ObjectFlow::setTransformation);
    parseSingleton(node, objectFlow, data, "selection", &ObjectFlow::setSelection, &ObjectFlow::setSelection);
}

void parseObjectNodeFeatures(YAML::Node node, ObjectNode& objectNode, ParserData& data) {
    parseSingleton(node, objectNode, data, "selection", &ObjectNode::setSelection, &ObjectNode::setSelection);
    parseSingleton(node, objectNode, data, "upperBound", &ObjectNode::setUpperBound, &ObjectNode::setUpperBound);
    parseBoolean(node, objectNode, "isControlType", &ObjectNode::setControlType);
    if (node["ordering"]) {
        if (!node["ordering"].IsScalar()) {
            throw SerializationError("ordering must be a scalar value! " + getLineNumber(node["ordering"]));
        }
        string orderingValue = node["ordering"].as<string>();
        if (orderingValue == "unordered") {
            objectNode.setOrdering(ObjectNodeOrderingKind::UNORDERED);
        } else if (orderingValue == "ordered") {
            objectNode.setOrdering(ObjectNodeOrderingKind::ORDERED);
        } else if (orderingValue == "LIFO") {
            objectNode.setOrdering(ObjectNodeOrderingKind::LIFO);
        } else if (orderingValue == "FIFO") {
            objectNode.setOrdering(ObjectNodeOrderingKind::FIFO);
        } else {
            throw SetStateException("Could not identify proper ordering for object node, must be one of these : [unordered, ordered, LIFO, FIFO] " + getLineNumber(node["ordering"]));
        }
    }
}

void parseOpaqueActionFeatures(YAML::Node node, OpaqueAction& opaqueAction, ParserData& data) {
    parseSet<InputPin>(node, opaqueAction, data, "inputValues", &OpaqueAction::getInputValues);
    parseSet<OutputPin>(node, opaqueAction, data, "outputValues", &OpaqueAction::getOutputValues);
    parseSet<LiteralString>(node, opaqueAction, data, "bodies", &OpaqueAction::getBodies);
}

void parseOpaqueBehaviorFeatures(YAML::Node node, OpaqueBehavior& opaqueBehavior, ParserData& data) {
    parseSet<LiteralString>(node, opaqueBehavior, data, "bodies", &OpaqueBehavior::getBodies);
}

void parsePackageFeatures(YAML::Node node, Package& pckg, ParserData& data) {
    parseSet<PackageableElement>(node, pckg, data, "packagedElements", &Package::getPackagedElements);
    if (node["ownedStereotypes"]) {
        if (!node["ownedStereotypes"].IsSequence()) {
            throw SerializationError("Could not parse set ownedStereotypes for Package, must be a sequence! " + getLineNumber(node["ownedStereotypes"]));
        }
        for (size_t i = 0; i < node["ownedStereotypes"].size(); i++) {
            if (node["ownedStereotypes"][i].IsMap()) {
                pckg.m_ownedStereotypes.add(parseNode(node["ownedStereotypes"][i], data)->as<Stereotype>());
            } else if (node["ownedStereotypes"][i].IsScalar()) {
                pckg.m_ownedStereotypes.add(ID::fromString(node["ownedStereotypes"][i].as<string>()));
            } else {
                throw SerializationError("could not parse ownedStereotypes entry for Package " + getLineNumber(node["ownedStereotypes"][i]));
            }
        }
    }
    parseSet<PackageMerge>(node, pckg, data, "packageMerges", &Package::getPackageMerge);
    parseSet<ProfileApplication>(node, pckg, data, "profileApplications", &Package::getProfileApplications);
}

void parsePackageImportFeatures(YAML::Node node, PackageImport& packageImport, ParserData& data) {
    parseSingleton<Package>(node, packageImport, data, "importedPackage", &PackageImport::setImportedPackage, &PackageImport::setImportedPackage);
    parseVisibilty(node, packageImport);
}

void parsePackageMergeFeatures(YAML::Node node, PackageMerge& packageMerge, ParserData& data) {
    parseSingleton<Package>(node, packageMerge, data, "mergedPackage", &PackageMerge::setMergedPackage, &PackageMerge::setMergedPackage);
}

void parseParameterFeatures(YAML::Node node, Parameter& parameter, ParserData& data) {
    // TODO string default
    if (node["direction"]) {
        if (!node["direction"].IsScalar()) {
            throw SerializationError("could not parse parameter direction because it was not a scalar value " + getLineNumber(node["direction"]));
        }
        string directionVal = node["direction"].as<string>();
        if (directionVal == "in") {
            parameter.setDirection(ParameterDirectionKind::IN_UML);
        } else if (directionVal == "inout") {
            parameter.setDirection(ParameterDirectionKind::INOUT);
        } else if (directionVal == "out") {
            parameter.setDirection(ParameterDirectionKind::OUT_UML);
        } else if (directionVal == "return") {
            parameter.setDirection(ParameterDirectionKind::RETURN);
        } else {
            throw SerializationError("could not parse parameter direction value " + getLineNumber(node["direction"]));
        }
    }
    if (node["effect"]) {
        string effectVal = node["effect"].as<string>();
        if (effectVal == "create") {
            parameter.setEffect(ParameterEffectKind::CREATE);
        } else if (effectVal == "read") {
            parameter.setEffect(ParameterEffectKind::READ);
        } else if (effectVal == "update") {
            parameter.setEffect(ParameterEffectKind::UPDATE);
        } else if (effectVal == "delete") {
            parameter.setEffect(ParameterEffectKind::DELETE_UML);
        } else {
            throw SerializationError("Could not parse parameter effect value " + getLineNumber(node["effect"]));
        }
    }
    parseBoolean(node, parameter, "isException", &Parameter::setIsException);
    parseBoolean(node, parameter, "isStream", &Parameter::setIsStream);
    parseSingleton<ValueSpecification>(node, parameter, data, "defaultValue", &Parameter::setDefaultValue, &Parameter::setDefaultValue);
    parseSet<ParameterSet>(node, parameter, data, "parameterSets", &Parameter::getParameterSets);
}

void parseParameterableElementFeatures(YAML::Node node, ParameterableElement& parameterableElement, ParserData& data) {
    parseSingleton<TemplateParameter>(node, parameterableElement, data, "templateParameter", &ParameterableElement::setTemplateParameter, &ParameterableElement::setTemplateParameter);
}

void parseParameterSetFeatures(YAML::Node node, ParameterSet& parameterSet, ParserData& data) {
    parseSet<Parameter>(node, parameterSet, data, "parameters", &ParameterSet::getParameters);
    parseSet<Constraint>(node, parameterSet, data, "conditions", &ParameterSet::getConditions);
}

void parsePinFeatures(YAML::Node node, Pin& pin, __attribute__((unused)) ParserData& data) {
    parseBoolean(node, pin, "isControl", &Pin::setIsControl);
}

void parsePortFeatures(YAML::Node node, Port& port, __attribute__((unused)) ParserData& data) {
    parseBoolean(node, port, "isBehavior", &Port::setIsBehavior);
    parseBoolean(node, port, "isConjugated", &Port::setIsConjugated);
    parseBoolean(node, port, "isService", &Port::setIsService);
    // parseSet<Port>(node, port, data, "redefinedPorts", &Port::getRedefinedPorts);
}

void parseProfileApplicationFeatures(YAML::Node node, ProfileApplication& profileApplication, ParserData& data) {
    parseSingleton<Profile>(node, profileApplication, data, "appliedProfile", &ProfileApplication::setAppliedProfile, &ProfileApplication::setAppliedProfile);
}

void parsePropertyFeatures(YAML::Node node, Property& property, ParserData& data) {
    if (node["aggregation"]) {
        string aggregationVal = node["aggregation"].as<string>();
        if (aggregationVal == "composite" || aggregationVal == "COMPOSITE") {
            property.setAggregation(AggregationKind::COMPOSITE);
        } else if (aggregationVal == "none" || aggregationVal == "NONE") {
            property.setAggregation(AggregationKind::NONE);
        } else if (aggregationVal == "shared" || aggregationVal == "SHARED") {
            property.setAggregation(AggregationKind::SHARED);
        } else {
            throw SerializationError("could not determine aggregation strategy for property " + getLineNumber(node["aggregation"]));
        }
    }
    parseBoolean(node, property, "isComposite", &Property::setComposite);
    // TODO flags
    parseSingleton<ValueSpecification>(node, property, data, "defaultValue", &Property::setDefaultValue, &Property::setDefaultValue);
    parseSingleton<Association>(node, property, data, "association", &Property::setAssociation, &Property::setAssociation);
    parseSet<Property>(node, property, data, "redefinedProperties", &Property::getRedefinedProperties);
    parseSet<Property>(node, property, data, "subsettedProperties", &Property::getSubsettedProperties);
}

void parseReceptionFeatures(YAML::Node node, Reception& reception, ParserData& data) {
    parseSingleton<Signal>(node, reception, data, "signal", &Reception::setSignal, &Reception::setSignal);
}

void parseRedefinableTemplateSignatureFeatures(YAML::Node node, RedefinableTemplateSignature& redefinableTemplateSignature, ParserData& data) {
    parseSet<RedefinableTemplateSignature>(node, redefinableTemplateSignature, data, "extendedSignatures", &RedefinableTemplateSignature::getExtendedSignatures);
}

void parseSignalFeatures(YAML::Node node, Signal& signal, ParserData& data) {
    parseSet<Property>(node, signal, data, "ownedAttributes", &Signal::getOwnedAttributes);
}

void parseSlotFeatures(YAML::Node node, Slot& slot, ParserData& data) {
    parseSet<ValueSpecification>(node, slot, data, "values", &Slot::getValues);
    parseSingleton<StructuralFeature>(node, slot, data, "definingFeature", &Slot::setDefiningFeature, &Slot::setDefiningFeature);
}

void parseStereotypeFeatures(YAML::Node node, Stereotype& stereotype, ParserData& data) {
    // TODO we might not need this but keeping it for now

}

void parseStructuralFeatureFeatures(YAML::Node node, StructuralFeature& structuralFeature, __attribute__((unused)) ParserData& data) {
    parseBoolean(node, structuralFeature, "isReadOnly", &StructuralFeature::setReadOnly);
}

void parseStructuredClassifierFeatures(YAML::Node node, StructuredClassifier& structuredClassifier, ParserData& data) {
    parseSet<Connector>(node, structuredClassifier, data, "ownedConnectors", &StructuredClassifier::getOwnedConnectors);
}

void parseTemplateableElementFeatures(YAML::Node node, TemplateableElement& templateableElement, ParserData& data) {
    parseSingleton<TemplateSignature>(node, templateableElement, data, "ownedTemplateSignature", &TemplateableElement::setOwnedTemplateSignature, &TemplateableElement::setOwnedTemplateSignature);
    parseSet<TemplateBinding>(node, templateableElement, data, "templateBindings", &TemplateableElement::getTemplateBindings);
}

void parseTemplateBindingFeatures(YAML::Node node, TemplateBinding& templateBinding, ParserData& data) {
    parseSet<TemplateParameterSubstitution>(node, templateBinding, data, "parameterSubstitutions", &TemplateBinding::getParameterSubstitutions);
    parseSingleton<TemplateSignature>(node, templateBinding, data, "signature", &TemplateBinding::setSignature, &TemplateBinding::setSignature);
}

void parseTemplateParameterFeatures(YAML::Node node, TemplateParameter& templateParameter, ParserData& data) {
    parseSingleton<ParameterableElement>(node, templateParameter, data, "ownedParameteredElement", &TemplateParameter::setOwnedParameteredElement, &TemplateParameter::setOwnedParameteredElement);
    parseSingleton<ParameterableElement>(node, templateParameter, data, "parameteredElement", &TemplateParameter::setParameteredElement, &TemplateParameter::setParameteredElement);
    parseSingleton<ParameterableElement>(node, templateParameter, data, "ownedDefault", &TemplateParameter::setOwnedDefault, &TemplateParameter::setOwnedDefault);
    parseSingleton<ParameterableElement>(node, templateParameter, data, "default", &TemplateParameter::setDefault, &TemplateParameter::setDefault);
}

void parseTemplateParameterSubstitutionFeatures(YAML::Node node, TemplateParameterSubstitution& templateParameterSubstitution, ParserData& data) {
    parseSingleton<TemplateParameter>(node, templateParameterSubstitution, data, "formal", &TemplateParameterSubstitution::setFormal, &TemplateParameterSubstitution::setFormal);
    parseSingleton<ParameterableElement>(node, templateParameterSubstitution, data, "ownedActual", &TemplateParameterSubstitution::setOwnedActual, &TemplateParameterSubstitution::setOwnedActual);
    parseSingleton<ParameterableElement>(node, templateParameterSubstitution, data, "actual", &TemplateParameterSubstitution::setActual, &TemplateParameterSubstitution::setActual);
}

void parseTemplateSignatureFeatures(YAML::Node node, TemplateSignature& templateSignature, ParserData& data) {
    parseSet<TemplateParameter>(node, templateSignature, data, "parameters", &TemplateSignature::getParameters);
    parseSet<TemplateParameter>(node, templateSignature, data, "ownedParameters", &TemplateSignature::getOwnedParameters);
}

void parseTypedElementFeatures(YAML::Node node, TypedElement& typedElement, ParserData& data) {
    parseSingleton<Type>(node, typedElement, data, "type", &TypedElement::setType, &TypedElement::setType);
}

void parseValuePinFeatures(YAML::Node node, ValuePin& valuePin, ParserData& data) {
    parseSingleton(node, valuePin, data, "value", &ValuePin::setValue, &ValuePin::setValue);
}

bool parseActivityEdgeScope(YAML::Node node, ActivityEdge& activityEdge, ParserData& data) {
    return parseSingleton(node, activityEdge, data, "activity", &ActivityEdge::setActivity, &ActivityEdge::setActivity);
}

bool parseActivityNodeScope(YAML::Node node, ActivityNode& activityNode, ParserData& data) {
    return parseSingleton(node, activityNode, data, "activity", &ActivityNode::setActivity, &ActivityNode::setActivity);
}

bool parseActivityGroupScope(YAML::Node node, ActivityGroup& activityGroup, ParserData& data) {
    return parseSingleton(node, activityGroup, data, "inActivity", &ActivityGroup::setInActivity, &ActivityGroup::setInActivity);
}

bool parseActivityPartitionScope(YAML::Node node, ActivityPartition& activityPartition, ParserData& data) {
    return parseSingleton(node, activityPartition, data, "superPartition", &ActivityPartition::setSuperPartition, &ActivityPartition::setSuperPartition);
}

bool parseConstraintScope(YAML::Node node, Constraint& constraint, ParserData& data) {
    return parseSingleton<Namespace>(node, constraint, data, "context", &Constraint::setContext, &Constraint::setContext);
}

bool parseDeploymentScope(YAML::Node node, Deployment& deployment, ParserData& data) {
    return parseSingleton(node, deployment, data, "location", &Deployment::setLocation, &Deployment::setLocation);
}

bool parseElementScope(YAML::Node node, Element& el, ParserData& data) {
    if (node["owner"]) {
        ID id = ID::fromString(node["owner"].as<string>());
        if (data.update){
            ElementPtr owner = data.manager->get(id);
            if (el.isSubClassOf(ElementType::VALUE_SPECIFICATION)) {
                if (owner->isSubClassOf(ElementType::SLOT)) {
                    Slot& slot = owner->as<Slot>();
                    
                    // ordered set, assume add to end
                    if (!slot.getValues().contains(el.getID())) {
                        slot.getValues().add(el.as<ValueSpecification>());
                    }
                }
            }

            // TODO

        } else {
            el.setOwner(id);
        }
        return true;
    }
    return false;
}

bool parseElementImportScope(YAML::Node node, ElementImport& elementImport, ParserData& data) {
    return parseSingleton<Namespace>(node, elementImport, data, "importingNamespace", &ElementImport::setImportingNamespace, &ElementImport::setImportingNamespace);
}

bool parseEnumerationLiteralScope(YAML::Node node, EnumerationLiteral& literal, __attribute__((unused)) ParserData& data) {
    if (node["enumeration"]) {
        if (!node["enumeration"].IsScalar()) {
            throw SerializationError("Could not parse enumeration literal's owning eneruation, must be a scalar " + getLineNumber(node["enumeration"]));
        }
        literal.setEnumeration(ID::fromString(node["enumeration"].as<string>()));
        return true;
    }
    return false;
}

bool parseExceptionHandlerScope(YAML::Node node, ExceptionHandler& exceptionHandler, ParserData& data) {
    return parseSingleton(node, exceptionHandler, data, "protectedNode", &ExceptionHandler::setProtectedNode, &ExceptionHandler::setProtectedNode);
}

bool parseGeneralizationScope(YAML::Node node, Generalization& generalization, ParserData& data) {
    return parseSingleton<Classifier>(node, generalization, data, "specific", &Generalization::setSpecific, &Generalization::setSpecific);
}

bool parseInterfaceRealizationScope(YAML::Node node , InterfaceRealization& interfaceRealization, ParserData& data) {
    return parseSingleton<BehavioredClassifier>(node, interfaceRealization, data, "implementingClassifier", &InterfaceRealization::setImplementingClassifier, &InterfaceRealization::setImplementingClassifier);
}

bool parseNamedElementScope(YAML::Node node, NamedElement& namedElement, __attribute__((unused)) ParserData& data) {
    if (node["namespace"]) {
        namedElement.setNamespace(ID::fromString(node["namespace"].as<string>()));
        return true;
    }
    return false;
}

bool parseOperationScope(YAML::Node node, Operation& operation, ParserData& data) {
    if (parseSingleton<Class>(node, operation, data, "class", &Operation::setClass, &Operation::setClass)) {
        return true;
    } else if (parseSingleton<DataType>(node, operation, data, "dataType", &Operation::setDataType, &Operation::setDataType)) {
        return true;
    } else if (parseSingleton<Interface>(node, operation, data, "interface", &Operation::setInterface, &Operation::setInterface)) {
        return true;
    }
    return false;
}

bool parsePackageableElementScope(YAML::Node node, PackageableElement& el, ParserData& data) {
    return parseSingleton<Package>(node, el, data, "owningPackage", &PackageableElement::setOwningPackage, &PackageableElement::setOwningPackage);
}

bool parsePackageImportScope(YAML::Node node, PackageImport& packageImport, ParserData& data) {
    return parseSingleton<Namespace>(node, packageImport, data, "importingNamespace", &PackageImport::setImportingNamespace, &PackageImport::setImportingNamespace);
}

bool parsePackageMergeScope(YAML::Node node, PackageMerge& packageMerge, ParserData& data) {
    return parseSingleton<Package>(node, packageMerge, data, "receivingPackage", &PackageMerge::setReceivingPackage, &PackageMerge::setReceivingPackage);
}

bool parseParameterScope(YAML::Node node, Parameter& parameter, ParserData& data) {
    return parseSingleton<Operation>(node, parameter, data, "operation", &Parameter::setOperation, &Parameter::setOperation);
}

bool parseParameterableElementScope(YAML::Node node, ParameterableElement& parameterableElement, ParserData& data) {
    return parseSingleton<TemplateParameter>(node, parameterableElement, data, "owningTemplateParameter", &ParameterableElement::setOwningTemplateParameter, &ParameterableElement::setOwningTemplateParameter);
}

bool parseProfileApplicationScope(YAML::Node node, ProfileApplication& profileApplication, ParserData& data) {
    return parseSingleton<Package>(node, profileApplication, data, "applyingPackage", &ProfileApplication::setApplyingPackage, &ProfileApplication::setApplyingPackage);
}

bool parsePropertyScope(YAML::Node node, Property& property, ParserData& data) {
    if (parseSingleton<Class>(node, property, data, "class", &Property::setClass, &Property::setClass)) {
        return true;
    }
    if (parseSingleton<DataType>(node, property, data, "dataType", &Property::setDataType, &Property::setDataType)) {
        return true;
    }
    if (parseSingleton<Interface>(node, property, data, "interface", &Property::setInterface, &Property::setInterface)) {
        return true;
    }
    if (parseSingleton<Association>(node, property, data, "owningAssociation", &Property::setOwningAssociation, &Property::setOwningAssociation)) {
        return true;
    }
    return false;
}

bool parseRedefinableTemplateSignatureScope(YAML::Node node, RedefinableTemplateSignature& redefinableTemplateSignature, ParserData& data) {
    return parseSingleton<Classifier>(node, redefinableTemplateSignature, data, "classifier", &RedefinableTemplateSignature::setClassifier, &RedefinableTemplateSignature::setClassifier);
}

bool parseSlotScope(YAML::Node node, Slot& slot, ParserData& data) {
    return parseSingleton<InstanceSpecification>(node, slot, data, "owningInstance", &Slot::setOwningInstance, &Slot::setOwningInstance);
}

bool parseTemplateBindingScope(YAML::Node node, TemplateBinding& templateBinding, ParserData& data) {
    return parseSingleton<TemplateableElement>(node, templateBinding, data, "boundElement", &TemplateBinding::setBoundElement, &TemplateBinding::setBoundElement);
}

bool parseTemplateParameterScope(YAML::Node node, TemplateParameter& templateParameter, ParserData& data) {
    return parseSingleton<TemplateSignature>(node, templateParameter, data, "signature", &TemplateParameter::setSignature, &TemplateParameter::setSignature);
}

bool parseTemplateParameterSubstitutionScope(YAML::Node node, TemplateParameterSubstitution& templateParameterSubstitution, ParserData& data) {
    return parseSingleton<TemplateBinding>(node, templateParameterSubstitution, data, "templateBinding", &TemplateParameterSubstitution::setTemplateBinding, &TemplateParameterSubstitution::setTemplateBinding);
}

bool parseTemplateSignatureScope(YAML::Node node, TemplateSignature& templateSignature, ParserData& data) {
    return parseSingleton<TemplateableElement>(node, templateSignature, data, "template", &TemplateSignature::setTemplate, &TemplateSignature::setTemplate);
}

}
