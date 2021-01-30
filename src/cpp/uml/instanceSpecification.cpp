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