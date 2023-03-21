#pragma once

#include "yaml-cpp/yaml.h"
#include "openUmlSerializationPolicy.h"
#include "uml/uml-stable.h"

namespace UML {
    // general functions for parsing
    ElementPtr parseNode(YAML::Node node, ParserData& data);
    std::string getLineNumber(YAML::Node node);

    // parsing features of element types
    void parseClassFeatures(YAML::Node node, Class& clazz, ParserData& data);
    void parseClassifierFeatures(YAML::Node node, Classifier& classifier, ParserData& data);
    void parseCommentFeatures(YAML::Node node, Comment& comment, ParserData& data);
    void parseConstraintFeatures(YAML::Node node, Constraint& constraint, ParserData& data);
    void parseDataTypeFeatures(YAML::Node node, DataType& dataType, ParserData& data);
    void parseDependencyFeatures(YAML::Node node, Dependency& dependency, ParserData& data);
    void parseElementFeatures(YAML::Node node, Element& el, ParserData& data);
    void parseElementImportFeatures(YAML::Node node, ElementImport& elementImport, ParserData& data);
    void parseEnumerationFeatures(YAML::Node node, Enumeration& enumeration, ParserData& data);
    void parseInstanceSpecificationFeatures(YAML::Node node, InstanceSpecification& inst, ParserData& data);
    void parseMultiplicityElementFeatures(YAML::Node node, MultiplicityElement& multiplicityElement, ParserData& data);
    void parseNamedElementFeatures(YAML::Node node, NamedElement& el, ParserData& data);
    void parseNamespaceFeatures(YAML::Node node, Namespace& nmspc, ParserData& data);
    void parsePackageFeatures(YAML::Node node, Package& pckg, ParserData& data);
    void parsePackageImportFeatures(YAML::Node node, PackageImport& packageImport, ParserData& data);
    void parseParameterableElementFeatures(YAML::Node node, ParameterableElement& parameterableElement, ParserData& data);
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
    bool parseNamedElementScope(YAML::Node node, NamedElement& namedElement, ParserData& data);
    bool parsePackageableElementScope(YAML::Node node, PackageableElement& el, ParserData& data);
    bool parsePackageImportScope(YAML::Node node, PackageImport& packageImport, ParserData& data);
    bool parseParameterableElementScope(YAML::Node node, ParameterableElement& parameterableElement, ParserData& data);
    bool parsePropertyScope(YAML::Node node, Property& property, ParserData& data);
    bool parseTemplateBindingScope(YAML::Node node, TemplateBinding& templateBinding, ParserData& data);
    bool parseTemplateParameterScope(YAML::Node node, TemplateParameter& templateParameter, ParserData& data);
    bool parseTemplateParameterSubstitutionScope(YAML::Node node, TemplateParameterSubstitution& templateParameterSubstitution, ParserData& data);
    bool parseTemplateSignatureScope(YAML::Node node, TemplateSignature& templateSignature, ParserData& data);
}