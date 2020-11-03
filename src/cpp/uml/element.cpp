#include "headers/element.h"

boost::uuids::uuid Element::getID(){
    return uuid;
}

void Element::setID(string id) {
    uuid = boost::lexical_cast<boost::uuids::uuid>(id);
}

list<Element*> Element::getOwnedElements() {
    return ownedElements;
}