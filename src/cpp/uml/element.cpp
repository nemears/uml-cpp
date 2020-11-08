#include "headers/element.h"

void Element::setID(string id) {
    uuid = boost::lexical_cast<boost::uuids::uuid>(id);
}