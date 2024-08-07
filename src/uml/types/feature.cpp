#include "uml/uml-stable.h"

using namespace UML;

void Feature::referenceErased(ID id) {
    RedefinableElement::referenceErased(id);
    NamedElement::referenceErased(id);
    eraseFromSet(id, m_featuringClassifier);
}

Singleton<Classifier, Feature>& Feature::getFeaturingClassifierSingleton() {
    return m_featuringClassifier;
}

Feature::Feature() : Element(ElementType::FEATURE) {
    m_featuringClassifier.opposite(&Classifier::getFeatures);
}

ClassifierPtr Feature::getFeaturingClassifier() const {
    return m_featuringClassifier.get();
}

// void Feature::setFeaturingClassifier(Classifier* clazz) {
//     m_featuringClassifier.set(clazz);
// }

// void Feature::setFeaturingClassifier(ID id) {
//     m_featuringClassifier.set(id);
// }

bool Feature::isStatic() {
    return m_static;
}

void Feature::setStatic(bool isStatic) {
    m_static = isStatic;
}

bool Feature::is(ElementType eType) const {
    bool ret = RedefinableElement::is(eType);

    if (!ret) {
        ret = eType == ElementType::FEATURE;
    }

    return ret;
}
