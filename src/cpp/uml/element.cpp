#include "uml/element.h"

void UML::Element::setID(string id) {
    if (UML::isValidUUID4(id)) {
        uuid = boost::lexical_cast<boost::uuids::uuid>(id);
    } else {
        throw invalidID_Exception;
    }
}