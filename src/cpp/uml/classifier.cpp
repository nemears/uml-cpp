#include "uml/classifier.h"

using namespace UML;

Classifier::Classifier() {
    m_attributes = new Sequence<Property>;
}

Classifier::~Classifier() {
    delete m_attributes;
}

Sequence<Property>& Classifier::getAttributes() {
    return *m_attributes;
}

bool Classifier::isPrimitive() {
    return false;
}

ElementType Classifier::getElementType() {
    return ElementType::CLASSIFIER;
}