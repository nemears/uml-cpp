#include "yuml-parsers/outputPinParser.h"

Element* OutputPinParser::createElement() {
    TypedElementParser::theEl = new OutputPin;
    return TypedElementParser::theEl;
}

bool OutputPinParser::emit(YAML::Emitter& emitter, Element* el) {
    if (el->getElementType() == ElementType::OUTPUT_PIN) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "outputPin";
        emitter << YAML::BeginMap;
    }

    bool ret = PinParser::emit(emitter, el);

    if (el->getElementType() == ElementType::OUTPUT_PIN) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}

OutputPinParser OutputPinParser::createNewParser() {
    return OutputPinParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
}