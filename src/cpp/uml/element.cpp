#include "uml/element.h"

using namespace UML;

void Element::setID(string id) {
    if (UML::isValidUUID4(id)) {
        uuid = boost::lexical_cast<boost::uuids::uuid>(id);
    } else {
        throw invalidID_Exception;
    }
}

ElementType Element::getElementType() {
    return ElementType::ELEMENT;
}