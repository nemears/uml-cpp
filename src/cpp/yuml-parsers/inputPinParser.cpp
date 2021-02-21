#include "yuml-parsers/inputPinParser.h"

Element* InputPinParser::createElement() {
    TypedElementParser::theEl = new InputPin;
    return TypedElementParser::theEl;
}

bool InputPinParser::emit(YAML::Emitter& emitter, Element* el) {
    if (el->getElementType() == ElementType::INPUT_PIN) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "inputPin";
        emitter << YAML::BeginMap;
    }

    bool ret = PinParser::emit(emitter, el);

    if (el->getElementType() == ElementType::INPUT_PIN) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}

InputPinParser InputPinParser::createNewParser() {
    return InputPinParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
}