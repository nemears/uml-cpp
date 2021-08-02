#include "uml/behavioredClassifier.h"

using namespace UML;

BehavioredClassifier::BehavioredClassifier() {

}

BehavioredClassifier::BehavioredClassifier(const BehavioredClassifier& classifier) {

}

BehavioredClassifier::~BehavioredClassifier() {

}

ElementType BehavioredClassifier::getElementType() const {
    return ElementType::BEHAVIORED_CLASSIFIER;
}

bool BehavioredClassifier::isSubClassOf(ElementType eType) const {
    bool ret = Classifier::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::BEHAVIORED_CLASSIFIER;
    }

    return ret;
}