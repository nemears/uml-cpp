#include "uml/feature.h"
#include "uml/classifier.h"
#include "uml/uml-stable.h"

using namespace UML;

void Feature::referenceReindexed(ID oldID, ID newID) {
    // RedefinableElement::referenceReindexed(oldID, newID);
    NamedElement::referenceReindexed(oldID, newID);
    m_featuringClassifier.reindex(oldID, newID);
}

void Feature::referencingReleased(ID id) {
    // RedefinableElement::referencingReleased(id);
    NamedElement::referencingReleased(id);
    m_featuringClassifier.release(id);
}

void Feature::restoreReferences() {
    // RedefinableElement::restoreReferences();
    NamedElement::restoreReferences();
    // m_featuringClassifier.restoreReference();
}

void Feature::referenceErased(ID id) {
    // RedefinableElement::referenceErased(id);
    NamedElement::referenceErased(id);
    m_featuringClassifier.eraseElement(id);
}

Set<Classifier, Feature>& Feature::getFeaturingClassifierSingleton() {
    return m_featuringClassifier;
}

void Feature::init() {
    m_featuringClassifier.opposite(&Classifier::getFeatures);
    m_featuringClassifier.m_signature = &Feature::getFeaturingClassifierSingleton;
}

void Feature::copy(const Feature& rhs) {
    m_featuringClassifier = rhs.m_featuringClassifier;
}

Feature::Feature() : Element(ElementType::FEATURE) {
    init();
}

Feature::Feature(const Feature& feature) : Element(ElementType::FEATURE) {
    // abstract
}

Classifier* Feature::getFeaturingClassifier() {
    return m_featuringClassifier.get();
}

Classifier& Feature::getFeaturingClassifierRef() {
    return m_featuringClassifier.getRef();
}

ID Feature::getFeaturingClassifierID() const {
    return m_featuringClassifier.id();
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

bool Feature::isSubClassOf(ElementType eType) const {
    bool ret = RedefinableElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::FEATURE;
    }

    return ret;
}