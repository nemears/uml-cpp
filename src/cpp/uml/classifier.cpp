#include "uml/classifier.h"
#include "uml/property.h"

using namespace UML;

Classifier::Classifier() {
    m_attributes = new Sequence<Property>;
    m_attributes->addProcedures.push_back(new AddAttributeFunctor(this));
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

void Classifier::AddAttributeFunctor::operator()(Element& el) const {
    if (dynamic_cast<Property&>(el).getClassifier() != m_el) {
        dynamic_cast<Property&>(el).setClassifier(dynamic_cast<Classifier*>(m_el));
    }

    if (dynamic_cast<Property&>(el).getNamespace() != m_el) {
        dynamic_cast<Property&>(el).setNamespace(dynamic_cast<Classifier*>(m_el));
    }
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