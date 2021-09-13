#include "uml/feature.h"
#include "uml/classifier.h"

using namespace std;
using namespace UML;

void Feature::RemoveFeaturingClassifierProcedure::operator()(Classifier* el) const {
    if (el->getFeatures().count(m_me->getID())) {
        el->getFeatures().remove(*m_me);
    }
}

void Feature::AddFeaturingClassifierProcedure::operator()(Classifier* el) const {
    if (!el->getFeatures().count(m_me->getID())) {
        el->getFeatures().add(*m_me);
    }
}

void Feature::reindexID(ID oldID, ID newID) {
    // if (m_featuringClassifier.has()) {
    //     m_featuringClassifier.get()->getFeatures().reindex(oldID, newID);
    // }
    // RedefinableElement::reindexID(oldID, newID);
    // NamedElement::reindexID(oldID, newID);
}

void Feature::reindexName(string oldName, string newName) {
    if (getFeaturingClassifier()) {
        getFeaturingClassifier()->getFeatures().reindex(m_id, oldName, newName);
    }
    NamedElement::reindexName(oldName, newName);
}

void Feature::referenceReindexed(ID oldID, ID newID) {
    RedefinableElement::referenceReindexed(oldID, newID);
    if (m_featuringClassifier.id() == oldID) {
        m_featuringClassifier.reindex(oldID, newID);
    }
}

Feature::Feature() {
    m_featuringClassifier.m_signature = &Feature::m_featuringClassifier;
    m_featuringClassifier.m_removeProcedures.push_back(new RemoveFeaturingClassifierProcedure(this));
    m_featuringClassifier.m_addProcedures.push_back(new AddFeaturingClassifierProcedure(this));
    m_static = false;
}

Feature::Feature(const Feature& feature) : RedefinableElement(feature), NamedElement(feature), Element(feature) {
    m_featuringClassifier = feature.m_featuringClassifier;
    m_featuringClassifier.m_me = this;
    m_featuringClassifier.m_removeProcedures.clear();
    m_featuringClassifier.m_addProcedures.clear();
    m_featuringClassifier.m_removeProcedures.push_back(new RemoveFeaturingClassifierProcedure(this));
    m_featuringClassifier.m_addProcedures.push_back(new AddFeaturingClassifierProcedure(this));
    m_static = feature.m_static;
}

Classifier* Feature::getFeaturingClassifier() {
    return m_featuringClassifier.get();
}

Classifier& Feature::getFeaturingClassifierRef() {
    return m_featuringClassifier.getRef();
}

bool Feature::hasFeaturingClassifier() const {
    return m_featuringClassifier.has();
}

void Feature::setFeaturingClassifier(Classifier* clazz) {
    m_featuringClassifier.set(clazz);
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

void Feature::restoreReleased(ID id, Element* released) {
    RedefinableElement::restoreReleased(id, released);
    if (m_featuringClassifier.id() == id) {
        if (!released->as<Classifier>().getFeatures().count(id)) {
            released->as<Classifier>().getFeatures().add(*this);
        }
    }
}

void Feature::referencingReleased(ID id) {
    RedefinableElement::referencingReleased(id);
    if (m_featuringClassifier.id() == id) {
        m_featuringClassifier.release();
    }
}