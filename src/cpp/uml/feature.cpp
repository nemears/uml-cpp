#include "uml/feature.h"
#include "uml/classifier.h"
#include "uml/universalFunctions.h"

using namespace std;
using namespace UML;

void Feature::reindexID(ID oldID, ID newID) {
    if (!m_featuringClassifierID.isNull()) {
        if (!m_featuringClassifierPtr) {
            m_featuringClassifierPtr = &m_manager->get<Classifier>(m_featuringClassifierID);
        }
        m_featuringClassifierPtr->getFeatures().reindex(oldID, newID);
    }

    NamedElement::reindexID(oldID, newID);
}

void Feature::reindexName(string oldName, string newName) {
    if (getFeaturingClassifier()) {
        getFeaturingClassifier()->getFeatures().reindex(m_id, oldName, newName);
    }
    
    NamedElement::reindexName(oldName, newName);
}

Feature::Feature() {
    m_featuringClassifierPtr = 0;
    m_static = false;
}

Feature::Feature(const Feature& feature) {
    m_featuringClassifierID = feature.m_featuringClassifierID;
    m_featuringClassifierPtr = feature.m_featuringClassifierPtr;
    m_static = feature.m_static;
}

Classifier* Feature::getFeaturingClassifier() {
    return universalGet<Classifier>(m_featuringClassifierID, m_featuringClassifierPtr, m_manager);
}

void Feature::setFeaturingClassifier(Classifier* clazz) {
    if (!m_featuringClassifierID.isNull()) {
        if (!m_featuringClassifierPtr) {
            m_featuringClassifierPtr = &m_manager->get<Classifier>(m_featuringClassifierID);
        }
        if (m_featuringClassifierPtr->getFeatures().count(m_id)) {
            m_featuringClassifierPtr->getFeatures().remove(*this);
        }
        m_featuringClassifierPtr = 0;
        m_featuringClassifierID = ID::nullID();
    }

    if (clazz) {
        m_featuringClassifierID = clazz->getID();
    }

    if (!m_manager) {
        m_featuringClassifierPtr = clazz;
    }

    if (clazz) {
        if (!clazz->getFeatures().count(m_id)) {
            clazz->getFeatures().add(*this);
        }
    }
}

bool Feature::isStatic() {
    return m_static;
}

void Feature::setStatic(bool isStatic) {
    m_static = isStatic;
}

ElementType Feature::getElementType() const {
    return ElementType::FEATURE;
}

bool Feature::isSubClassOf(ElementType eType) const {
    bool ret = RedefinableElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::FEATURE;
    }

    return ret;
}