#include "element.h"

using namespace std;

boost::uuids::uuid Element::getID(){
    return uuid;
}

void Element::setId(string id) {
    uuid = boost::lexical_cast<boost::uuids::uuid>(id);
}

list<Element> Element::getOwnedElements() {
    return ownedElements;
}