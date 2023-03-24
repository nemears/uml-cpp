#pragma once

#include "yaml-cpp/yaml.h"
#include "openUmlSerializationPolicy.h"
#include "uml/uml-stable.h"

namespace UML {
    // general functions for parsing
    ElementPtr parseNode(YAML::Node node, ParserData& data);
    std::string getLineNumber(YAML::Node node);

    // parsing features of element types
    void parseBehavioralFeatureFeatures(YAML::Node node, BehavioralFeature& behavioralFeature, ParserData& data);
    void parseClassFeatures(YAML::Node node, Class& clazz, ParserData& data);
    void parseClassifierFeatures(YAML::Node node, Classifier& classifier, ParserData& data);
    void parseClassifierTemplateParameterFeatures(YAML::Node node, ClassifierTemplateParameter& classifierTemplateParameter, ParserData& data);
    void parseCommentFeatures(YAML::Node node, Comment& comment, ParserData& data);
    void parseConstraintFeatures(YAML::Node node, Constraint& constraint, ParserData& data);
    void parseDataTypeFeatures(YAML::Node node, DataType& dataType, ParserData& data);
    void parseDependencyFeatures(YAML::Node node, Dependency& dependency, ParserData& data);
    void parseElementFeatures(YAML::Node node, Element& el, ParserData& data);
    void parseElementImportFeatures(YAML::Node node, ElementImport& elementImport, ParserData& data);
    void parseEnumerationFeatures(YAML::Node node, Enumeration& enumeration, ParserData& data);
    void parseExpressionFeatures(YAML::Node node, Expression& expression, ParserData& data);
    void parseFeatureFeatures(YAML::Node node, Feature& feature, ParserData& data);
    void parseGeneralizationFeatures(YAML::Node node, Generalization& generalization, ParserData& data);
    void parseGeneralizationSetFeatures(YAML::Node node, GeneralizationSet& generalizationSet, ParserData& data);
    void parseInstanceSpecificationFeatures(YAML::Node node, InstanceSpecification& inst, ParserData& data);
    void parseInstanceValueFeatures(YAML::Node node, InstanceValue& instanceValue, ParserData& data);
    void parseLiteralBoolFeatures(YAML::Node node, LiteralBool& literalBool, ParserData& data);
    void parseLiteralIntegerFeatures(YAML::Node node, LiteralInt& literalInt, ParserData& data);
    void parseLiteralRealFeatures(YAML::Node node, LiteralReal& literalReal, ParserData& data);
    void parseLiteralStringFeatures(YAML::Node node, LiteralString& literalString, ParserData& data);
    void parseLiteralUnlimitedNaturalfeatures(YAML::Node node, LiteralUnlimitedNatural& literalUnlimitedNatural, ParserData& data);
    void parseMultiplicityElementFeatures(YAML::Node node, MultiplicityElement& multiplicityElement, ParserData& data);
    void parseNamedElementFeatures(YAML::Node node, NamedElement& el, ParserData& data);
    void parseNamespaceFeatures(YAML::Node node, Namespace& nmspc, ParserData& data);
    void parsePackageFeatures(YAML::Node node, Package& pckg, ParserData& data);
    void parsePackageImportFeatures(YAML::Node node, PackageImport& packageImport, ParserData& data);
    void parseParameterFeatures(YAML::Node node, Parameter& parameter, ParserData& data);
    void parseParameterableElementFeatures(YAML::Node node, ParameterableElement& parameterableElement, ParserData& data);
    void parseParameterSetFeatures(YAML::Node node, ParameterSet& parameterSet, ParserData& data);
    void parsePropertyFeatures(YAML::Node node, Property& property, ParserData& data);
    void parseRedefinableTemplateSignatureFeatures(YAML::Node node, RedefinableTemplateSignature& redefinableTemplateSignature, ParserData& data);
    void parseSlotFeatures(YAML::Node node, Slot& slot, ParserData& data);
    void parseStructuralFeatureFeatures(YAML::Node node, StructuralFeature& structuralFeature, ParserData& data);
    void parseStructuredClassifierFeatures(YAML::Node node, StructuredClassifier& structuredClassifier, ParserData& data);
    void parseTemplateableElementFeatures(YAML::Node node, TemplateableElement& templateableElement, ParserData& data);
    void parseTemplateBindingFeatures(YAML::Node node, TemplateBinding& templateBinding, ParserData& data);
    void parseTemplateParameterFeatures(YAML::Node node, TemplateParameter& templateParameter, ParserData& data);
    void parseTemplateParameterSubstitutionFeatures(YAML::Node node, TemplateParameterSubstitution& templateParameterSubstitution, ParserData& data);
    void parseTemplateSignatureFeatures(YAML::Node node, TemplateSignature& templateSignature, ParserData& data);
    void parseTypedElementFeatures(YAML::Node node, TypedElement& typedElement, ParserData& data);

    // parsing scopes of element types
    bool parseConstraintScope(YAML::Node node, Constraint& constraint, ParserData& data);
    bool parseElementScope(YAML::Node node, Element& el, ParserData& data);
    bool parseElementImportScope(YAML::Node node, ElementImport& elementImport, ParserData& data);
    bool parseEnumerationLiteralScope(YAML::Node node, EnumerationLiteral& literal, ParserData& data);
    bool parseGeneralizationScope(YAML::Node node, Generalization& generalization, ParserData& data);
    bool parseNamedElementScope(YAML::Node node, NamedElement& namedElement, ParserData& data);
    bool parseOperationScope(YAML::Node node, Operation& operation, ParserData& data);
    bool parsePackageableElementScope(YAML::Node node, PackageableElement& el, ParserData& data);
    bool parsePackageImportScope(YAML::Node node, PackageImport& packageImport, ParserData& data);
    bool parseParameterableElementScope(YAML::Node node, ParameterableElement& parameterableElement, ParserData& data);
    bool parsePropertyScope(YAML::Node node, Property& property, ParserData& data);
    bool parseRedefinableTemplateSignatureScope(YAML::Node node, RedefinableTemplateSignature& redefinableTemplateSignature, ParserData& data);
    bool parseSlotScope(YAML::Node node, Slot& slot, ParserData& data);
    bool parseTemplateBindingScope(YAML::Node node, TemplateBinding& templateBinding, ParserData& data);
    bool parseTemplateParameterScope(YAML::Node node, TemplateParameter& templateParameter, ParserData& data);
    bool parseTemplateParameterSubstitutionScope(YAML::Node node, TemplateParameterSubstitution& templateParameterSubstitution, ParserData& data);
    bool parseTemplateSignatureScope(YAML::Node node, TemplateSignature& templateSignature, ParserData& data);
}