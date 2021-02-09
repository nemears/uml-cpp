#include "uml/classifier.h"

// list<Property*> Classifier::getOwnedAttributes() {
//     return ownedAttributes;
// }

using namespace UML;

// bool Classifier::isPrimitive() {
//     return false;
// }

ElementType Classifier::getElementType() {
    return ElementType::CLASSIFIER;
}

void Classifier::addAttribute(Property& p) {
    ownedAttributes.push_back(&p);
}

void Classifier::removeAttribute(Property& p) {
    list<Property*>::iterator i = ownedAttributes.begin();
    while (i != ownedAttributes.end()) {
        if ((*i)->uuid == p.uuid) {
            ownedAttributes.erase(i);
            break;
        }
        ++i;
    }
}