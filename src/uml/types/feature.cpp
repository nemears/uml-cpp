#include "uml/uml-stable.h"

using namespace UML;

Singleton<Classifier, Feature>& Feature::getFeaturingClassifierSingleton() {
    return m_featuringClassifier;
}

Feature::Feature(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    RedefinableElement(elementType, manager)
{
    m_featuringClassifier.opposite(&Classifier::getFeatures);
}

ClassifierPtr Feature::getFeaturingClassifier() const {
    return m_featuringClassifier.get();
}

bool Feature::isStatic() {
    return m_static;
}

void Feature::setStatic(bool isStatic) {
    m_static = isStatic;
}
