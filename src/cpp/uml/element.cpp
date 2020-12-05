#include "headers/element.h"

void Element::setID(string id) {
    if (regex_match (id, regex("[0-9a-f]{8}-[0-9a-f]{4}-4[0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}"))) {
        uuid = boost::lexical_cast<boost::uuids::uuid>(id);
    } else {
        throw invalidID_Exception;
    }
}