#pragma once

#include "yaml-cpp/yaml.h"
#include "openUmlSerializationPolicy.h"
#include "uml/uml-stable.h"

namespace UML {
    // general functions for parsing
    ElementPtr parseNode(YAML::Node node, ParserData& data);
    std::string getLineNumber(YAML::Node node);
    ElementType elementTypeFromString(std::string eType);

    // parsing features of element types
    // void parseActionFeatures(YAML::Node node, Action& action, ParserData& data);
    // void parseActionInputPinFeatures(YAML::Node node, ActionInputPin& actionInputPin, ParserData& data);
    // void parseActivityFeatures(YAML::Node node, Activity& activity, ParserData& data);
    // void parseActivityEdgeFeatures(YAML::Node node, ActivityEdge& activityEdge, ParserData& data);
    // void parseActivityNodeFeatures(YAML::Node node, ActivityNode& activityNode, ParserData& data);
    // void parseActivityParameterNodeFeatures(YAML::Node node, ActivityParameterNode& activityParameterNode, ParserData& data);
    // void parseActivityPartitionFeatures(YAML::Node node, ActivityPartition& activityPartition, ParserData& data);
    // void parseArtifactFeatures(YAML::Node node, Artifact& artifact, ParserData& data);
    // void parseAssociationFeatures(YAML::Node node, Association& association, ParserData& data);
    // void parseBehaviorFeatures(YAML::Node node, Behavior& behavior, ParserData& data);
    // void parseBehavioralFeatureFeatures(YAML::Node node, BehavioralFeature& behavioralFeature, ParserData& data);
    // void parseBehavioredClassifierFeatures(YAML::Node node, BehavioredClassifier& behavioredClassifier, ParserData& data);
    // void parseCallActionFeatures(YAML::Node node, CallAction& callAction, ParserData& data);
    // void parseCallBehaviorActionFeatures(YAML::Node node, CallBehaviorAction& callBehaviorAction, ParserData& data);
    // void parseClassFeatures(YAML::Node node, Class& clazz, ParserData& data);
    // void parseClassifierFeatures(YAML::Node node, Classifier& classifier, ParserData& data);
    // void parseClassifierTemplateParameterFeatures(YAML::Node node, ClassifierTemplateParameter& classifierTemplateParameter, ParserData& data);
    // void parseCommentFeatures(YAML::Node node, Comment& comment, ParserData& data);
    // void parseConnectorFeatures(YAML::Node node, Connector& connector, ParserData& data);
    // void parseConnectorEndFeatures(YAML::Node node, ConnectorEnd& connectorEnd, ParserData& data);
    // void parseConstraintFeatures(YAML::Node node, Constraint& constraint, ParserData& data);
    // void parseDataTypeFeatures(YAML::Node node, DataType& dataType, ParserData& data);
    // void parseDecisionNodeFeatures(YAML::Node node, DecisionNode& decisionNode, ParserData& data);
    // void parseDependencyFeatures(YAML::Node node, Dependency& dependency, ParserData& data);
    // void parseDeploymentFeatures(YAML::Node node, Deployment& deployment, ParserData& data);
    // void parseDeploymentTargetFeatures(YAML::Node node, DeploymentTarget& deploymentTarget, ParserData& data);
    void parseElementFeatures(YAML::Node node, Element& el, ParserData& data);
    // void parseElementImportFeatures(YAML::Node node, ElementImport& elementImport, ParserData& data);
    // void parseEnumerationFeatures(YAML::Node node, Enumeration& enumeration, ParserData& data);
    // void parseExceptionHandlerFeatures(YAML::Node node, ExceptionHandler& exceptionHandler, ParserData& data);
    // void parseExecutableNodeFeatures(YAML::Node node, ExecutableNode& executableNode, ParserData& data);
    // void parseExpressionFeatures(YAML::Node node, Expression& expression, ParserData& data);
    // void parseExtensionFeatures(YAML::Node node, Extension& extension, ParserData& data);
    // void parseFeatureFeatures(YAML::Node node, Feature& feature, ParserData& data);
    // void parseGeneralizationFeatures(YAML::Node node, Generalization& generalization, ParserData& data);
    // void parseGeneralizationSetFeatures(YAML::Node node, GeneralizationSet& generalizationSet, ParserData& data);
    // void parseInstanceSpecificationFeatures(YAML::Node node, InstanceSpecification& inst, ParserData& data);
    // void parseInstanceValueFeatures(YAML::Node node, InstanceValue& instanceValue, ParserData& data);
    // void parseInterfaceFeatures(YAML::Node node, Interface& interface, ParserData& data);
    // void parseInterfaceRealizationFeatures(YAML::Node node, InterfaceRealization& interfaceRealization, ParserData& data);
    // void parseInvocationActionFeatures(YAML::Node node, InvocationAction& invocationAction, ParserData& data);
    // void parseJoinNodeFeatures(YAML::Node node, JoinNode& joinNode, ParserData& data);
    // void parseLiteralBoolFeatures(YAML::Node node, LiteralBool& literalBool, ParserData& data);
    // void parseLiteralIntegerFeatures(YAML::Node node, LiteralInt& literalInt, ParserData& data);
    // void parseLiteralRealFeatures(YAML::Node node, LiteralReal& literalReal, ParserData& data);
    // void parseLiteralStringFeatures(YAML::Node node, LiteralString& literalString, ParserData& data);
    // void parseLiteralUnlimitedNaturalFeatures(YAML::Node node, LiteralUnlimitedNatural& literalUnlimitedNatural, ParserData& data);
    // void parseManifestationFeatures(YAML::Node node, Manifestation& manifestation, ParserData& data);
    // void parseModelFeatures(YAML::Node node, Model& model, ParserData& data);
    // void parseMultiplicityElementFeatures(YAML::Node node, MultiplicityElement& multiplicityElement, ParserData& data);
    void parseNamedElementFeatures(YAML::Node node, NamedElement& el, ParserData& data);
    void parseNamespaceFeatures(YAML::Node node, Namespace& nmspc, ParserData& data);
    // void parseObjectFlowFeatures(YAML::Node node, ObjectFlow& objectFlow, ParserData& data);
    // void parseObjectNodeFeatures(YAML::Node node, ObjectNode& objectNode, ParserData& data);
    // void parseOpaqueActionFeatures(YAML::Node node, OpaqueAction& opaqueAction, ParserData& data);
    // void parseOpaqueBehaviorFeatures(YAML::Node node, OpaqueBehavior& opaqueBehavior, ParserData& data);
    void parsePackageFeatures(YAML::Node node, Package& pckg, ParserData& data);
    // void parsePackageImportFeatures(YAML::Node node, PackageImport& packageImport, ParserData& data);
    // void parsePackageMergeFeatures(YAML::Node node, PackageMerge& packageMerge, ParserData& data);
    // void parseParameterFeatures(YAML::Node node, Parameter& parameter, ParserData& data);
    // void parseParameterableElementFeatures(YAML::Node node, ParameterableElement& parameterableElement, ParserData& data);
    // void parseParameterSetFeatures(YAML::Node node, ParameterSet& parameterSet, ParserData& data);
    // void parsePinFeatures(YAML::Node node, Pin& pin, ParserData& data);
    // void parsePortFeatures(YAML::Node node, Port& port, ParserData& data);
    // void parseProfileApplicationFeatures(YAML::Node node, ProfileApplication& profileApplication, ParserData& data);
    // void parsePropertyFeatures(YAML::Node node, Property& property, ParserData& data);
    // void parseReceptionFeatures(YAML::Node node, Reception& reception, ParserData& data);
    // void parseRedefinableTemplateSignatureFeatures(YAML::Node node, RedefinableTemplateSignature& redefinableTemplateSignature, ParserData& data);
    // void parseSignalFeatures(YAML::Node node, Signal& signal, ParserData& data);
    // void parseSlotFeatures(YAML::Node node, Slot& slot, ParserData& data);
    // void  parseStereotypeFeatures(YAML::Node node, Stereotype& stereotype, ParserData& data);
    // void parseStructuralFeatureFeatures(YAML::Node node, StructuralFeature& structuralFeature, ParserData& data);
    // void parseStructuredClassifierFeatures(YAML::Node node, StructuredClassifier& structuredClassifier, ParserData& data);
    // void parseTemplateableElementFeatures(YAML::Node node, TemplateableElement& templateableElement, ParserData& data);
    // void parseTemplateBindingFeatures(YAML::Node node, TemplateBinding& templateBinding, ParserData& data);
    // void parseTemplateParameterFeatures(YAML::Node node, TemplateParameter& templateParameter, ParserData& data);
    // void parseTemplateParameterSubstitutionFeatures(YAML::Node node, TemplateParameterSubstitution& templateParameterSubstitution, ParserData& data);
    // void parseTemplateSignatureFeatures(YAML::Node node, TemplateSignature& templateSignature, ParserData& data);
    // void parseTypedElementFeatures(YAML::Node node, TypedElement& typedElement, ParserData& data);
    // void parseValuePinFeatures(YAML::Node node, ValuePin& valuePin, ParserData& data);

    // parsing scopes of element types
    // bool parseActivityEdgeScope(YAML::Node node, ActivityEdge& activityEdge, ParserData& data);
    // bool parseActivityNodeScope(YAML::Node node, ActivityNode& activityNode, ParserData& data);
    // bool parseActivityGroupScope(YAML::Node node, ActivityGroup& activityGroup, ParserData& data);
    // bool parseActivityPartitionScope(YAML::Node node, ActivityPartition& activityPartition, ParserData& data);
    // bool parseConstraintScope(YAML::Node node, Constraint& constraint, ParserData& data);
    // bool parseDeploymentScope(YAML::Node node, Deployment& deployment, ParserData& data);
    bool parseElementScope(YAML::Node node, Element& el, ParserData& data);
    // bool parseElementImportScope(YAML::Node node, ElementImport& elementImport, ParserData& data);
    // bool parseEnumerationLiteralScope(YAML::Node node, EnumerationLiteral& literal, ParserData& data);
    // bool parseExceptionHandlerScope(YAML::Node node, ExceptionHandler& exceptionHandler, ParserData& data);
    // bool parseGeneralizationScope(YAML::Node node, Generalization& generalization, ParserData& data);
    // bool parseInterfaceRealizationScope(YAML::Node node , InterfaceRealization& interfaceRealization, ParserData& data);
    bool parseNamedElementScope(YAML::Node node, NamedElement& namedElement, ParserData& data);
    // bool parseOperationScope(YAML::Node node, Operation& operation, ParserData& data);
    bool parsePackageableElementScope(YAML::Node node, PackageableElement& el, ParserData& data);
    // bool parsePackageImportScope(YAML::Node node, PackageImport& packageImport, ParserData& data);
    // bool parsePackageMergeScope(YAML::Node node, PackageMerge& packageMerge, ParserData& data);
    // bool parseParameterScope(YAML::Node node, Parameter& parameter, ParserData& data);
    // bool parseParameterableElementScope(YAML::Node node, ParameterableElement& parameterableElement, ParserData& data);
    // bool parseProfileApplicationScope(YAML::Node node, ProfileApplication& profileApplication, ParserData& data);
    // bool parsePropertyScope(YAML::Node node, Property& property, ParserData& data);
    // bool parseRedefinableTemplateSignatureScope(YAML::Node node, RedefinableTemplateSignature& redefinableTemplateSignature, ParserData& data);
    // bool parseSlotScope(YAML::Node node, Slot& slot, ParserData& data);
    // bool parseTemplateBindingScope(YAML::Node node, TemplateBinding& templateBinding, ParserData& data);
    // bool parseTemplateParameterScope(YAML::Node node, TemplateParameter& templateParameter, ParserData& data);
    // bool parseTemplateParameterSubstitutionScope(YAML::Node node, TemplateParameterSubstitution& templateParameterSubstitution, ParserData& data);
    // bool parseTemplateSignatureScope(YAML::Node node, TemplateSignature& templateSignature, ParserData& data);
}
