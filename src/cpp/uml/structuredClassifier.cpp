#include "uml/structuredClassifier.h"

using namespace UML;

StructuredClassifier::StructuredClassifier() {
    m_ownedAttributes = new Sequence<Property>;
}

StructuredClassifier::~StructuredClassifier() {
    delete m_ownedAttributes;
}

Sequence<Property>& StructuredClassifier::getOwnedAttributes() {
    return *m_ownedAttributes;
}

ElementType StructuredClassifier::getElementType() {
    return ElementType::STRUCTURED_CLASSIFIER;
}

bool StructuredClassifier::isSubClassOf(ElementType eType) {
    bool ret = Classifier::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::STRUCTURED_CLASSIFIER;
    }

    return ret;
}