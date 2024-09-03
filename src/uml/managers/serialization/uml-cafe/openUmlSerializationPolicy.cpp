#include "uml/managers/serialization/uml-cafe/openUmlSerializationPolicy.h"
#include "uml/managers/abstractManager.h"
#include "uml/managers/serialization/uml-cafe/emit.h"
#include "uml/managers/serialization/uml-cafe/parse.h"

using namespace std;

namespace UML {

std::string UmlCafeSerializationPolicy::extensionName() {
    return ".yml";
}

AbstractElementPtr UmlCafeSerializationPolicy::parseIndividual(std::string data, AbstractManager& manager) {
    ParserData metaData;
    metaData.manager = &manager;
    return parse(data, metaData);
}

AbstractElementPtr UmlCafeSerializationPolicy::parseWhole(std::string data, AbstractManager& manager) {
    ParserData metaData;
    metaData.mode = SerializationMode::WHOLE;
    metaData.manager = &manager;
    return parse(data, metaData);
}

std::string UmlCafeSerializationPolicy::emitIndividual(AbstractElement& el, __attribute__((unused))AbstractManager& manager) {
    EmitterData data;
    return emit(el, data);
}

std::string UmlCafeSerializationPolicy::emitWhole(AbstractElement& el, __attribute__((unused))AbstractManager& manager) {
    EmitterData data;
    data.mode = SerializationMode::WHOLE;
    return emit(el, data);
}

std::string UmlCafeJsonSerializationPolicy::extensionName() {
    return ".json"; // ".uml.json"
}

AbstractElementPtr UmlCafeJsonSerializationPolicy::parseIndividual(std::string data, AbstractManager& manager) {
    ParserData metaData;
    metaData.manager = &manager;
    ElementPtr ret = parse(data, metaData);

    return ret;
}

AbstractElementPtr UmlCafeJsonSerializationPolicy::parseWhole(std::string data, AbstractManager& manager) {
    ParserData metaData;
    metaData.mode = SerializationMode::WHOLE;
    metaData.manager = &manager;
    return parse(data, metaData);
}

std::string UmlCafeJsonSerializationPolicy::emitIndividual(AbstractElement& el, __attribute__((unused))AbstractManager& manager) {
    EmitterData data;
    data.isJSON = true;
    return emit(el, data);
}

std::string UmlCafeJsonSerializationPolicy::emitWhole(AbstractElement& el, __attribute__((unused))AbstractManager& manager) {
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

AbstractElementPtr parse(std::string data, ParserData& metaData) {
    std::vector<YAML::Node> rootNodes = YAML::LoadAll(data);
    if (rootNodes.empty()) {
        throw SerializationError("could not parse data supplied to manager! Is it JSON or YAML?");
    }
    return parseNode(rootNodes[0], metaData);
}

ElementPtr parseNode(YAML::Node node, ParserData& data) {
    return 0;
}

std::string emit(AbstractElement& el, EmitterData& data) {
    return "";
}

void emitElementData(YAML::Emitter& emitter, Element& el, EmitterData& data) {}

}
