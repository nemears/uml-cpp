#include "uml/classifier.h"

using namespace UML;

bool Classifier::isPrimitive() {
    return false;
}

ElementType Classifier::getElementType() {
    return ElementType::CLASSIFIER;
}