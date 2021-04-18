#include "uml/feature.h"
#include "uml/classifier.h"

using namespace UML;

Classifier* Feature::getFeaturingClassifier() {
    return m_featuringClassifier;
}

void Feature::setFeaturingClassifier(Classifier* clazz) {
    m_featuringClassifier = clazz;
}

bool Feature::isStatic() {
    return m_static;
}

void Feature::setStatic(bool isStatic) {
    m_static = isStatic;
}

ElementType Feature::getElementType() {
    return ElementType::FEATURE;
}

bool Feature::isSubClassOf(ElementType eType) {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::FEATURE;
    }

    return ret;
}