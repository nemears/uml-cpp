#include "uml/managers/serialization/uml-cafe/openUmlSerializationPolicy.h"
#include "uml/managers/serialization/uml-cafe/emit.h"
#include "uml/managers/serialization/uml-cafe/parse.h"

using namespace std;

namespace UML {

std::string OpenUmlSerializationPolicy::extensionName() {
    return ".yml";
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

std::string OpenUmlSerializationPolicy::emitIndividual(Element& el, __attribute__((unused))AbstractManager& manager) {
    EmitterData data;
    return emit(el, data);
}

std::string OpenUmlSerializationPolicy::emitWhole(Element& el, __attribute__((unused))AbstractManager& manager) {
    EmitterData data;
    data.mode = SerializationMode::WHOLE;
    return emit(el, data);
}

std::string OpenUmlJsonSerializationPolicy::extensionName() {
    return ".json"; // ".uml.json"
}

ElementPtr OpenUmlJsonSerializationPolicy::parseIndividual(std::string data, AbstractManager& manager) {
    ParserData metaData;
    metaData.manager = &manager;
    ElementPtr ret = parse(data, metaData);

    return ret;
}

ElementPtr OpenUmlJsonSerializationPolicy::parseWhole(std::string data, AbstractManager& manager) {
    ParserData metaData;
    metaData.mode = SerializationMode::WHOLE;
    metaData.manager = &manager;
    return parse(data, metaData);
}

std::string OpenUmlJsonSerializationPolicy::emitIndividual(Element& el, __attribute__((unused))AbstractManager& manager) {
    EmitterData data;
    data.isJSON = true;
    return emit(el, data);
}

std::string OpenUmlJsonSerializationPolicy::emitWhole(Element& el, __attribute__((unused))AbstractManager& manager) {
    EmitterData data;
    data.mode = SerializationMode::WHOLE;
    data.isJSON = true;
    return emit(el, data);
}

std::string emit(Element& el, EmitterData& data) {
    YAML::Emitter emitter;
    if (data.isJSON) {
        emitter << YAML::DoubleQuoted << YAML::Flow;
    }
    emitter << YAML::BeginMap;
    emitElementData(emitter, el, data);
    emitter << YAML::EndMap;
    return emitter.c_str();
}

ElementPtr parse(std::string data, ParserData& metaData) {
    std::vector<YAML::Node> rootNodes = YAML::LoadAll(data);
    if (rootNodes.empty()) {
        throw SerializationError("could not parse data supplied to manager! Is it JSON or YAML?");
    }
    return parseNode(rootNodes[0], metaData);
}

}
