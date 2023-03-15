#include "uml/types/feature.h"
#include "uml/types/behavior.h"
#include "uml/types/package.h"
#include "uml/types/property.h"
#include "uml/types/generalization.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

void Feature::referenceReindexed(ID newID) {
    RedefinableElement::referenceReindexed(newID);
    NamedElement::referenceReindexed(newID);
    m_featuringClassifier.reindex(newID);
}

void Feature::referenceErased(ID id) {
    RedefinableElement::referenceErased(id);
    NamedElement::referenceErased(id);
    m_featuringClassifier.eraseElement(id);
}

// void Feature::restoreReference(Element* el) {
//     RedefinableElement::restoreReference(el);
//     m_featuringClassifier.restore(el);
// }

TypedSet<Classifier, Feature>& Feature::getFeaturingClassifierSingleton() {
    return m_featuringClassifier;
}

Feature::Feature() : Element(ElementType::FEATURE) {
    m_featuringClassifier.opposite(&Classifier::getFeatures);
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