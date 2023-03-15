#include "uml/managers/serialization/open_uml/openUmlSerializationPolicy.h"

namespace UML {

std::string OpenUmlSerializationPolicy::extensionName() {
    return ".yml"; // ".uml.yml"
}

ElementPtr OpenUmlSerializationPolicy::parseIndividual(std::string data, AbstractManager& manager) {
    // TODO
    return ElementPtr();
}

ElementPtr OpenUmlSerializationPolicy::parseWhole(std::string data, AbstractManager& manager) {
    // TODO
    return ElementPtr();
}

std::string OpenUmlSerializationPolicy::emitIndividual(Element& el, AbstractManager& manager) {
    EmitterData data;
    return emit(el, data);
}

std::string OpenUmlSerializationPolicy::emitWhole(Element& el, AbstractManager& manager) {
    EmitterData data;
    data.mode = SerializationMode::WHOLE;
    return emit(el, data);
    return "";
}

std::string emit(Element& el, EmitterData& data) {
        // TODO
        return "";
    }
}