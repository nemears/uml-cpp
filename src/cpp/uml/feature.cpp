#include "uml/feature.h"
#include "uml/classifier.h"

using namespace std;
using namespace UML;

void Feature::reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) {
    if (m_featuringClassifier) {
        m_featuringClassifier->getFeatures().reindex(oldID, newID);
    }

    NamedElement::reindexID(oldID, newID);
}

void Feature::reindexName(string oldName, string newName) {
    if (m_featuringClassifier) {
        m_featuringClassifier->getFeatures().reindex(m_id, oldName, newName);
    }
    
    NamedElement::reindexName(oldName, newName);
}

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
    bool ret = RedefinableElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::FEATURE;
    }

    return ret;
}