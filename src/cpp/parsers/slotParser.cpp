#include "headers/slotParser.h"
#include "../uml/headers/slot.h"

using namespace UML;

Element* SlotParser::createElement() {
    return new Slot;
}

bool SlotParser::parseFeatures(YAML::Node node, Element* el) {

    if (node["value"]) {
        
    }

    return ElementParser::parseFeatures(node, el);
}