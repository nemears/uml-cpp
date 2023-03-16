#include "uml/managers/serialization/open_uml/openUmlSerializationPolicy.h"
#include "uml/managers/serialization/open_uml/emit.h"
#include "uml/managers/serialization/open_uml/parse.h"
#include "yaml-cpp/yaml.h"

namespace UML {

std::string OpenUmlSerializationPolicy::extensionName() {
    return ".yml"; // ".uml.yml"
}

ElementPtr OpenUmlSerializationPolicy::parseIndividual(std::string data, AbstractManager& manager) {
    ParserData metaData;
    metaData.manager = &manager;
    return parse(data, metaData);
}

ElementPtr OpenUmlSerializationPolicy::parseWhole(std::string data, AbstractManager& manager) {
    ParserData metaData;
    metaData.mode = SerializationMode::WHOLE;
    metaData.manager = &manager;
    return parse(data, metaData);
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
    YAML::Emitter emitter;
    if (data.isJSON) {
        emitter << YAML::DoubleQuoted << YAML::Flow;
        if (data.emitReferences) {
            emitter << YAML::BeginMap;
        }
    } else if (data.emitReferences) {
        emitter << YAML::BeginDoc;
    }
    emitter << YAML::BeginMap;
    // TODO emit scope
    emitElementBody(emitter, el, data);
    emitter << YAML::EndMap;
    if (data.emitReferences) {
        if (data.isJSON) {
            emitter << YAML::EndMap;
        } else {
            emitter << YAML::EndDoc;
        }
    }
    return emitter.c_str();
}

ElementPtr parse(std::string data, ParserData& metaData) {
    std::vector<YAML::Node> rootNodes = YAML::LoadAll(data);
    ElementPtr ret;
    if (rootNodes.empty()) {
        throw SerializationError("could not parse data supplied to manager! Is it JSON or YAML?");
    }
    if (metaData.mode == SerializationMode::INDIVIDUAL) {
        ret = parseNode(rootNodes[0], metaData);
        // TODO parseScope
        if (rootNodes.size() == 2) {
            // TODO set references from list
        }
    } else {
        ret = parseNode(rootNodes[0], metaData);
    }
    return ret;
}

}