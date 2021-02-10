#include "uml/instanceSpecification.h"

using namespace UML;

Classifier* InstanceSpecification::getClassifier() {
    return classifier;
}

void InstanceSpecification::setClassifier(Classifier* classifier) {
    this->classifier = classifier;
}

ElementType InstanceSpecification::getElementType() {
    return ElementType::INSTANCE_SPECIFICATION;
}

void InstanceSpecification::addSlot(Slot& slot) {
    slots.push_back(&slot);
}

void InstanceSpecification::removeSlot(Slot& slot) {
    list<Slot*>::iterator i = slots.begin();
    while (i != slots.end()) {
        if ((*i)->uuid == slot.uuid) {
            slots.erase(i);
            break;
        }
        ++i;
    }
}