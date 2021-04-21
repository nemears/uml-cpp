#include "uml/feature.h"
#include "uml/classifier.h"

using namespace UML;

Feature::Feature() {
    m_featuringClassifier = 0;
    m_static = false;
}

Feature::Feature(const Feature& feature) {
    m_featuringClassifier = feature.m_featuringClassifier;
    m_static = feature.m_static;
}

Classifier* Feature::getFeaturingClassifier() {
    return m_featuringClassifier;
}

void Feature::setFeaturingClassifier(Classifier* clazz) {
    m_featuringClassifier = clazz;
    if (m_featuringClassifier) {
        if (!m_featuringClassifier->getFeatures().count(m_id)) {
            m_featuringClassifier->getFeatures().add(*this);
        }
    }
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
    bool ret = NamedElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::FEATURE;
    }

    return ret;
}