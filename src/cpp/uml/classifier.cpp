#include "uml/classifier.h"

using namespace UML;

Classifier::Classifier() {
    m_attributes = new Sequence<Property>;
}

Classifier::~Classifier() {
    delete m_attributes;
}

void Classifier::reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) {
    Namespace::reindexID(oldID, newID);
}

void Classifier::reindexName(string oldName, string newName) {
    Namespace::reindexName(oldName, newName);
}

string Classifier::getName() {
    return Namespace::getName();
}

void Classifier::setName(const string& name) {
    Namespace::setName(name);
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