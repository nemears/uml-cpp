#include "uml/createObjectAction.h"

using namespace UML;

Classifier* CreateObjectAction::getClassifier() {
    return classifier;
}

void CreateObjectAction::setClassifier(Classifier* classifier) {
    this->classifier = classifier;
}

ElementType CreateObjectAction::getElementType() {
    return ElementType::CREATE_OBJECT_ACTION;
}