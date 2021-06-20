#include "uml/createObjectAction.h"

using namespace UML;

Classifier* CreateObjectAction::getClassifier() {
    return classifier;
}

void CreateObjectAction::setClassifier(Classifier* classifier) {
    this->classifier = classifier;
}

ElementType CreateObjectAction::getElementType() const {
    return ElementType::CREATE_OBJECT_ACTION;
}

bool CreateObjectAction::isSubClassOf(ElementType eType) const {
    bool ret = Action::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::CREATE_OBJECT_ACTION;
    }

    return ret;
}