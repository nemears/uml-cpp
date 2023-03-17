#pragma once

#include "yaml-cpp/yaml.h"
#include "openUmlSerializationPolicy.h"
#include "uml/uml-stable.h"

namespace UML {
    // general functions for emitting
    void emitElementData(YAML::Emitter& emitter, Element& el, EmitterData& data);

    // emit features of element types
    void emitDataTypeFeatures(YAML::Emitter& emitter, DataType& dataType, EmitterData& data);
    void emitElementFeatures(YAML::Emitter& emitter, Element& el, EmitterData& data);
    void emitInstanceSpecificationFeatures(YAML::Emitter& emitter, InstanceSpecification& specification, EmitterData& data);
    void emitNamedElementFeatures(YAML::Emitter& emitter, NamedElement& el, EmitterData& data);
    void emitPackageFeatures(YAML::Emitter& emitter, Package& package, EmitterData& data);
    void emitTypedElementFeatures(YAML::Emitter& emitter, TypedElement& typedElement, EmitterData& data);

    // emit scopes of element types
    bool emitElementScope(YAML::Emitter& emitter, Element& el, EmitterData& data);
    bool emitEnumerationLiteralScope(YAML::Emitter& emitter, EnumerationLiteral& literal, EmitterData& data);
    bool emitPackageableElementScope(YAML::Emitter& emitter, PackageableElement& el, EmitterData& data);
    bool emitPropertyScope(YAML::Emitter& emitter, Property& property, EmitterData& data);
}