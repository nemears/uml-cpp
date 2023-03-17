#pragma once

#include "yaml-cpp/yaml.h"
#include "openUmlSerializationPolicy.h"
#include "uml/uml-stable.h"

namespace UML {
    // general functions for emitting
    void emitElementBody(YAML::Emitter& emitter, Element& el, EmitterData& data);
    void emitScope(YAML::Emitter& emitter, Element& el, EmitterData& data);

    // emit features of element types
    void emitElement(YAML::Emitter& emitter, Element& el, EmitterData& data);
    void emitInstanceSpecificationFeatures(YAML::Emitter& emitter, InstanceSpecification& specification, EmitterData& data);
    void emitNamedElement(YAML::Emitter& emitter, NamedElement& el, EmitterData& data);

    // emitting elements
    void emitEnumerationLiteral(YAML::Emitter& emitter, EnumerationLiteral& literal, EmitterData& data);
    void emitInstanceSpecification(YAML::Emitter& emitter, InstanceSpecification& inst, EmitterData& data);

    // emitting sets
    void determineAndEmitInstanceSpecification(YAML::Emitter& emitter, InstanceSpecification& inst, EmitterData& data);
}