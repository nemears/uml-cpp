#include "uml/createObjectAction.h"

using namespace UML;

CreateObjectAction::CreateObjectAction() : Element(ElementType::CREATE_OBJECT_ACTION) {}

CreateObjectAction::CreateObjectAction(const CreateObjectAction& rhs) :
Action(rhs),
NamedElement(rhs),
Element(rhs, ElementType::CREATE_OBJECT_ACTION) {}

Classifier* CreateObjectAction::getClassifier() {
    return classifier;
}

void CreateObjectAction::setClassifier(Classifier* classifier) {
    this->classifier = classifier;
}

bool CreateObjectAction::isSubClassOf(ElementType eType) const {
    bool ret = Action::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::CREATE_OBJECT_ACTION;
    }

    return ret;
}