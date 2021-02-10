#include "uml/parameter.h"

using namespace UML;

ElementType Parameter::getElementType() {
    return ElementType::PARAMETER;
}

ParameterDirectionKind Parameter::getDirection() {
    return direction;
}

void Parameter::setDirection(ParameterDirectionKind direction) {
    this->direction = direction;
}

string Parameter::getDirectionString() {
    switch(direction) {
        case ParameterDirectionKind::IN : {
            return "IN";
        } case ParameterDirectionKind::OUT : {
            return "OUT";
        } case ParameterDirectionKind::INOUT : {
            return "INOUT";
        } case ParameterDirectionKind::RETURN : {
            return "RETURN";
        } default : {
            return "NONE";
        }
    }
}

void Parameter::setDirectionString(string& directionString) {
if (directionString.compare("IN") == 0) {
    setDirection(ParameterDirectionKind::IN);
} else if (directionString.compare("INOUT") == 0) {
    setDirection(ParameterDirectionKind::INOUT);
} else if (directionString.compare("OUT") == 0) {
    setDirection(ParameterDirectionKind::OUT);
} else if (directionString.compare("RETURN") == 0) {
    setDirection(ParameterDirectionKind::RETURN);
} else if (directionString.compare("NONE") == 0) {
    setDirection(ParameterDirectionKind::NONE);
} else {
    throw invalidDirectionException;
}
}