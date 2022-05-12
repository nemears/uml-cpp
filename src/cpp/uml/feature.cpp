#include "uml/feature.h"
#include "uml/behavior.h"
#include "uml/package.h"
#include "uml/property.h"
#include "uml/generalization.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

void Feature::referenceReindexed(ID oldID, ID newID) {
    RedefinableElement::referenceReindexed(oldID, newID);
    NamedElement::referenceReindexed(oldID, newID);
    m_featuringClassifier.reindex(oldID, newID);
}

void Feature::reindexName(ID id, std::string newName) {
    RedefinableElement::reindexName(id, newName);
    NamedElement::reindexName(id, newName);
}

void Feature::referencingReleased(ID id) {
    RedefinableElement::referencingReleased(id);
    NamedElement::referencingReleased(id);
    m_featuringClassifier.release(id);
}

void Feature::referenceErased(ID id) {
    RedefinableElement::referenceErased(id);
    NamedElement::referenceErased(id);
    m_featuringClassifier.eraseElement(id);
}

void Feature::restoreReference(Element* el) {
    RedefinableElement::restoreReference(el);
    m_featuringClassifier.restore(el);
}

Set<Classifier, Feature>& Feature::getFeaturingClassifierSingleton() {
    return m_featuringClassifier;
}

void Feature::init() {
    m_featuringClassifier.opposite(&Classifier::getFeatures);
}

Feature::Feature() : Element(ElementType::FEATURE) {
    init();
}

ClassifierPtr Feature::getFeaturingClassifier() const {
    return m_featuringClassifier.get();
}

void Feature::setFeaturingClassifier(Classifier* clazz) {
    m_featuringClassifier.set(clazz);
}

void Feature::setFeaturingClassifier(ID id) {
    m_featuringClassifier.set(id);
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