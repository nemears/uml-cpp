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