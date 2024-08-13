#pragma once

#include "yaml-cpp/yaml.h"
#include "openUmlSerializationPolicy.h"
#include "uml/uml-stable.h"

namespace UML {
    // general functions for emitting
    void emitElementData(YAML::Emitter& emitter, Element& el, EmitterData& data);

    // emit features of element types
    // void emitActionFeatures(YAML::Emitter& emitter, Action& action, EmitterData& data);
    // void emitActionInputPinFeatures(YAML::Emitter& emitter, ActionInputPin& actionInputPin, EmitterData& data);
    // void emitActivityFeatures(YAML::Emitter& emitter, Activity& activity, EmitterData& data);
    // void emitActivityEdgeFeatures(YAML::Emitter& emitter, ActivityEdge& activityEdge, EmitterData& data);
    // void emitActivityNodeFeatures(YAML::Emitter& emitter, ActivityNode& activityNode, EmitterData& data);
    // void emitActivityParameterNodeFeatures(YAML::Emitter& emitter, ActivityParameterNode& activityParameterNode, EmitterData& data);
    // void emitActivityPartitionFeatures(YAML::Emitter& emitter, ActivityPartition& activityPartition, EmitterData& data);
    void emitArtifactFeatures(YAML::Emitter& emitter, Artifact& artifact, EmitterData& data);
    void emitAssociationFeatures(YAML::Emitter& emitter, Association& association, EmitterData& data);
    void emitBehaviorFeatures(YAML::Emitter& emitter, Behavior& behavior, EmitterData& data);
    void emitBehavioralFeatureFeatures(YAML::Emitter& emitter, BehavioralFeature& behavioralFeature, EmitterData& data);
    void emitBehavioredClassifierFeatures(YAML::Emitter& emitter, BehavioredClassifier& behavioredClassifier, EmitterData& data);
    // void emitCallActionFeatures(YAML::Emitter& emitter, CallAction& callAction, EmitterData& data);
    // void emitCallBehaviorActionFeatures(YAML::Emitter& emitter, CallBehaviorAction& callBehaviorAction, EmitterData& data);
    void emitClassFeatures(YAML::Emitter& emitter, Class& clazz, EmitterData& data);
    void emitClassifierFeatures(YAML::Emitter& emitter, Classifier& classifier, EmitterData& data);
    void emitClassifierTemplateParameter(YAML::Emitter& emitter, ClassifierTemplateParameter& classifierTemplateParameter, EmitterData& data);
    void emitCommentFeatures(YAML::Emitter& emitter, Comment& comment, EmitterData& data);
    void emitConnectorFeatures(YAML::Emitter& emitter, Connector& connector, EmitterData& data);
    void emitConnectorEndFeatures(YAML::Emitter& emitter, ConnectorEnd& connectorEnd, EmitterData& data);
    void emitConstraintFeatures(YAML::Emitter& emitter, Constraint& constraint, EmitterData& data);
    void emitDataTypeFeatures(YAML::Emitter& emitter, DataType& dataType, EmitterData& data);
    // void emitDecisionNodeFeatures(YAML::Emitter& emitter, DecisionNode& decisionNode, EmitterData& data);
    void emitDependencyFeatures(YAML::Emitter& emitter, Dependency& dependency, EmitterData& data);
    void emitDeploymentFeatures(YAML::Emitter& emitter, Deployment& deployment, EmitterData& data);
    void emitDeploymentTargetFeatures(YAML::Emitter& emitter, DeploymentTarget& deploymentTarget, EmitterData& data);
    void emitElementFeatures(YAML::Emitter& emitter, Element& el, EmitterData& data);
    void emitElementImportFeatures(YAML::Emitter& emitter, ElementImport& elementImport, EmitterData& data);
    void emitEnumerationFeatures(YAML::Emitter& emitter, Enumeration& enumeration, EmitterData& data);
    // void emitExceptionHandlerFeatures(YAML::Emitter& emitter, ExceptionHandler& exceptionHandler, EmitterData& data);
    // void emitExecutableNodeFeatures(YAML::Emitter& emitter, ExecutableNode& executableNode, EmitterData& data);
    void emitExpressionFeatures(YAML::Emitter& emitter, Expression& expression, EmitterData& data);
    void emitExtensionFeatures(YAML::Emitter& emitter, Extension& extension, EmitterData& data);
    void emitFeatureFeatures(YAML::Emitter& emitter, Feature& feature, EmitterData& data);
    void emitGeneralizationFeatures(YAML::Emitter& emitter, Generalization& generalization, EmitterData& data);
    void emitGeneralizationSetFeatures(YAML::Emitter& emitter, GeneralizationSet& generalizationSet, EmitterData& data);
    void emitInstanceSpecificationFeatures(YAML::Emitter& emitter, InstanceSpecification& specification, EmitterData& data);
    void emitInstanceValueFeatures(YAML::Emitter& emitter, InstanceValue& instanceValue, EmitterData& data);
    void emitInterfaceFeatures(YAML::Emitter& emitter, Interface& interface, EmitterData& data);
    void emitInterfaceRealizationFeatures(YAML::Emitter& emitter, InterfaceRealization& interfaceRealization, EmitterData& data);
    // void emitInvocationActionFeatures(YAML::Emitter& emitter, InvocationAction& invocationAction, EmitterData& data);
    // void emitJoinNodeFeatures(YAML::Emitter& emitter, JoinNode& joinNode, EmitterData& data);
    void emitLiteralBoolFeatures(YAML::Emitter& emitter, LiteralBool& literalBool, EmitterData& data);
    void emitLiteralIntFeatures(YAML::Emitter& emitter, LiteralInt& literalInt, EmitterData& data);
    void emitLiteralRealFeatures(YAML::Emitter& emitter, LiteralReal& literalReal, EmitterData& data);
    void emitLiteralStringFeatures(YAML::Emitter& emitter, LiteralString& literalString, EmitterData& data);
    void emitLiteralUnlimitedNaturalFeatures(YAML::Emitter& emitter, LiteralUnlimitedNatural& literalUnlimitedNatural, EmitterData& data);
    void emitManifestationFeatures(YAML::Emitter& emitter, Manifestation& manifestation, EmitterData& data);
    void emitModelFeatures(YAML::Emitter& emitter, Model& model, EmitterData& data);
    void emitMultiplicityElementFeatures(YAML::Emitter& emitter, MultiplicityElement& multiplicityElement, EmitterData& data);
    void emitNamedElementFeatures(YAML::Emitter& emitter, NamedElement& el, EmitterData& data);
    void emitNamedElementDeploymentTargetFeatures(YAML::Emitter& emitter, NamedElement& el, EmitterData& data);
    void emitNamespaceFeatures(YAML::Emitter& emitter, Namespace& nmspc, EmitterData& data);
    // void emitObjectFlowFeatures(YAML::Emitter& emitter, ObjectFlow& objectFlow, EmitterData& data);
    // void emitObjectNodeFeatures(YAML::Emitter& emitter, ObjectNode& objectNode, EmitterData& data);
    // void emitOpaqueActionFeatures(YAML::Emitter& emitter, OpaqueAction& opaqueAction, EmitterData& data);
    void emitOpaqueBehaviorFeatures(YAML::Emitter& emitter, OpaqueBehavior& opaqueBehavior, EmitterData& data);
    void emitPackageFeatures(YAML::Emitter& emitter, Package& package, EmitterData& data);
    void emitPackageImportFeatures(YAML::Emitter& emitter, PackageImport& packageImport, EmitterData& data);
    void emitPackageMergeFeatures(YAML::Emitter& emitter, PackageMerge& packageMerge, EmitterData& data);
    void emitParameterFeatures(YAML::Emitter& emitter, Parameter& parameter, EmitterData& data);
    void emitParameterableElementFeatures(YAML::Emitter& emitter, ParameterableElement& parameterableElement, EmitterData& data);
    void emitParameterSetFeatures(YAML::Emitter& emitter, ParameterSet& parameterSet, EmitterData& data);
    // void emitPinFeatures(YAML::Emitter& emitter, Pin& pin, EmitterData& data);
    void emitPortFeatures(YAML::Emitter& emitter, Port& port, EmitterData& data);
    void emitProfileApplicationFeatures(YAML::Emitter& emitter, ProfileApplication& profileApplication, EmitterData& data);
    void emitPropertyFeatures(YAML::Emitter& emitter, Property& property, EmitterData& data);
    void emitReceptionFeatures(YAML::Emitter& emitter, Reception& reception, EmitterData& data);
    void emitRedefinableTemplateSignatureFeatures(YAML::Emitter& emitter, RedefinableTemplateSignature& redefinableTemplateSignature, EmitterData& data);
    void emitSignalFeatures(YAML::Emitter& emitter, Signal& signal, EmitterData& data);
    void emitSlotFeatures(YAML::Emitter& emitter, Slot& slot, EmitterData& data);
    void emitStereotypeFeatures(YAML::Emitter& emitter, Stereotype& stereotype, EmitterData& data);
    void emitStructuralFeatureFeatures(YAML::Emitter& emitter, StructuralFeature& structuralFeature, EmitterData& data);
    void emitStructuredClassifierFeatures(YAML::Emitter& emitter, StructuredClassifier& structuredClassifier, EmitterData& data);
    void emitTemplateableElementFeatures(YAML::Emitter& emitter, TemplateableElement& templateableElement, EmitterData& data);
    void emitTemplateBindingFeatures(YAML::Emitter& emitter, TemplateBinding& templateBinding, EmitterData& data);
    void emitTemplateParameterFeatures(YAML::Emitter& emitter, TemplateParameter& templateParameter, EmitterData& data);
    void emitTemplateParameterSubstitutionFeatures(YAML::Emitter& emitter, TemplateParameterSubstitution& templateParameterSubstitution, EmitterData& data);
    void emitTemplateSignatureFeatures(YAML::Emitter& emitter, TemplateSignature& signature, EmitterData& data);
    void emitTypedElementFeatures(YAML::Emitter& emitter, TypedElement& typedElement, EmitterData& data);
    // void emitValuePinFeatures(YAML::Emitter& emitter, ValuePin& valuePin, EmitterData& data);

    // emit scopes of element types
    // bool emitActivityEdgeScope(YAML::Emitter& emitter, ActivityEdge& activityEdge, EmitterData& data);
    // bool emitActivityNodeScope(YAML::Emitter& emitter, ActivityNode& activityNode, EmitterData& data);
    // bool emitActivityGroupScope(YAML::Emitter& emitter, ActivityGroup& activityGroup, EmitterData& data);
    // bool emitActivityPartitionScope(YAML::Emitter& emitter, ActivityPartition& activityPartition, EmitterData& data);
    bool emitConstraintScope(YAML::Emitter& emitter, Constraint& constraint, EmitterData& data);
    bool emitDeploymentScope(YAML::Emitter& emitter, Deployment& deployment, EmitterData& data);
    bool emitElementScope(YAML::Emitter& emitter, Element& el, EmitterData& data);
    bool emitElementImportScope(YAML::Emitter& emitter, ElementImport& elementImport, EmitterData& data);
    bool emitEnumerationLiteralScope(YAML::Emitter& emitter, EnumerationLiteral& literal, EmitterData& data);
    // bool emitExceptionHandlerScope(YAML::Emitter& emitter, ExceptionHandler& exceptionHandler, EmitterData& data);
    bool emitGeneralizationScope(YAML::Emitter& emitter, Generalization& generalization, EmitterData& data);
    bool emitInterfaceRealizationScope(YAML::Emitter& emitter, InterfaceRealization& interfaceRealization, EmitterData& data);
    bool emitNamedElementScope(YAML::Emitter& emitter, NamedElement& namedElement, EmitterData& data);
    bool emitOperationScope(YAML::Emitter& emitter, Operation& operation, EmitterData& data);
    bool emitPackageableElementScope(YAML::Emitter& emitter, PackageableElement& el, EmitterData& data);
    bool emitPackageImportScope(YAML::Emitter& emitter, PackageImport& packageImport, EmitterData& data);
    bool emitParameterScope(YAML::Emitter& emitter, Parameter& parameter, EmitterData& data);
    bool emitPackageMergeScope(YAML::Emitter& emitter, PackageMerge& packageMerge, EmitterData& data);
    bool emitParameterableElementScope(YAML::Emitter& emitter, ParameterableElement& el, EmitterData& data);
    bool emitProfileApplicationScope(YAML::Emitter& emitter, ProfileApplication& profileApplication, EmitterData& data);
    bool emitPropertyScope(YAML::Emitter& emitter, Property& property, EmitterData& data);
    bool emitRedefinableTemplateSignatureScope(YAML::Emitter& emitter, RedefinableTemplateSignature& redefinableTemplateSignature, EmitterData& data);
    bool emitSlotScope(YAML::Emitter& emitter, Slot& slot, EmitterData& data);
    bool emitTemplateBindingScope(YAML::Emitter& emitter, TemplateBinding& templateBinding, EmitterData& data);
    bool emitTemplateParameterScope(YAML::Emitter& emitter, TemplateParameter& templateParameter, EmitterData& data);
    bool emitTemplateParameterSubstitutionScope(YAML::Emitter& emitter, TemplateParameterSubstitution& templateParameterSubstitution, EmitterData& data);
    bool emitTemplateSignatureScope(YAML::Emitter& emitter, TemplateSignature& signature, EmitterData& data);
}
