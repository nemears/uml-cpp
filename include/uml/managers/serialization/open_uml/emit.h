#pragma once

#include "yaml-cpp/yaml.h"
#include "openUmlSerializationPolicy.h"

namespace UML {
    void emitElementBody(YAML::Emitter& emitter, Element& el, EmitterData& data);
}