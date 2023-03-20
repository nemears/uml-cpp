#pragma once

#include "yaml-cpp/yaml.h"
#include "openUmlSerializationPolicy.h"
#include "uml/uml-stable.h"

namespace UML {
    // general functions for emitting
    void emitElementData(YAML::Emitter& emitter, Element& el, EmitterData& data);

    // emit features of element types
    void emitClassFeatures(YAML::Emitter& emitter, Class& clazz, EmitterData& data);
    void emitClassifierFeatures(YAML::Emitter& emitter, Classifier& classifier, EmitterData& data);
    void emitCommentFeatures(YAML::Emitter& emitter, Comment& comment, EmitterData& data);
    void emitDataTypeFeatures(YAML::Emitter& emitter, DataType& dataType, EmitterData& data);
    void emitElementFeatures(YAML::Emitter& emitter, Element& el, EmitterData& data);
    void emitElementImportFeatures(YAML::Emitter& emitter, ElementImport& elementImport, EmitterData& data);
    void emitEnumerationFeatures(YAML::Emitter& emitter, Enumeration& enumeration, EmitterData& data);
    void emitInstanceSpecificationFeatures(YAML::Emitter& emitter, InstanceSpecification& specification, EmitterData& data);
    void emitNamedElementFeatures(YAML::Emitter& emitter, NamedElement& el, EmitterData& data);
    void emitNamespaceFeatures(YAML::Emitter& emitter, Namespace& nmspc, EmitterData& data);
    void emitPackageFeatures(YAML::Emitter& emitter, Package& package, EmitterData& data);
    void emitPackageImportFeatures(YAML::Emitter& emitter, PackageImport& packageImport, EmitterData& data);
    void emitParameterableElementFeatures(YAML::Emitter& emitter, ParameterableElement& parameterableElement, EmitterData& data);
    void emitStructuredClassifierFeatures(YAML::Emitter& emitter, StructuredClassifier& structuredClassifier, EmitterData& data);
    void emitTemplateableElementFeatures(YAML::Emitter& emitter, TemplateableElement& templateableElement, EmitterData& data);
    void emitTemplateBindingFeatures(YAML::Emitter& emitter, TemplateBinding& templateBinding, EmitterData& data);
    void emitTemplateParameterFeatures(YAML::Emitter& emitter, TemplateParameter& templateParameter, EmitterData& data);
    void emitTemplateParameterSubstitutionFeatures(YAML::Emitter& emitter, TemplateParameterSubstitution& templateParameterSubstitution, EmitterData& data);
    void emitTemplateSignatureFeatures(YAML::Emitter& emitter, TemplateSignature& signature, EmitterData& data);
    void emitTypedElementFeatures(YAML::Emitter& emitter, TypedElement& typedElement, EmitterData& data);

    // emit scopes of element types
    bool emitElementScope(YAML::Emitter& emitter, Element& el, EmitterData& data);
    bool emitElementImportScope(YAML::Emitter& emitter, ElementImport& elementImport, EmitterData& data);
    bool emitEnumerationLiteralScope(YAML::Emitter& emitter, EnumerationLiteral& literal, EmitterData& data);
    bool emitNamedElementScope(YAML::Emitter& emitter, NamedElement& namedElement, EmitterData& data);
    bool emitPackageableElementScope(YAML::Emitter& emitter, PackageableElement& el, EmitterData& data);
    bool emitPackageImportScope(YAML::Emitter& emitter, PackageImport& packageImport, EmitterData& data);
    bool emitParameterableElementScope(YAML::Emitter& emitter, ParameterableElement& el, EmitterData& data);
    bool emitPropertyScope(YAML::Emitter& emitter, Property& property, EmitterData& data);
    bool emitTemplateBindingScope(YAML::Emitter& emitter, TemplateBinding& templateBinding, EmitterData& data);
    bool emitTemplateParameterScope(YAML::Emitter& emitter, TemplateParameter& templateParameter, EmitterData& data);
    bool emitTemplateParameterSubstitutionScope(YAML::Emitter& emitter, TemplateParameterSubstitution& templateParameterSubstitution, EmitterData& data);
    bool emitTemplateSignatureScope(YAML::Emitter& emitter, TemplateSignature& signature, EmitterData& data);
}