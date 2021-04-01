#include "uml/classifier.h"
#include "uml/property.h"
#include "uml/generalization.h"

using namespace UML;

Classifier::Classifier() {
    m_attributes = new Sequence<Property>;
    m_attributes->addProcedures.push_back(new AddAttributeFunctor(this));
    m_attributes->removeProcedures.push_back(new RemoveAttributeFunctor(this));
    m_generalizations = new Sequence<Generalization>;
    m_generalizations->addProcedures.push_back(new AddGeneralizationFunctor(this));
    m_generalizations->addChecks.push_back(new CheckGeneralizationFunctor(this));
    m_generals = new Sequence<Classifier>;
    m_generals->addProcedures.push_back(new AddGeneralFunctor(this));
}

Classifier::~Classifier() {
    delete m_attributes;
    delete m_generalizations;
    delete m_generals;
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

void Classifier::RemoveAttributeFunctor::operator()(Element& el) const {
    if (dynamic_cast<Property&>(el).getClassifier() == m_el) {
        dynamic_cast<Property&>(el).setClassifier(0);
    }

    if (dynamic_cast<Property&>(el).getNamespace() == m_el) {
        if (dynamic_cast<Property&>(el).getNamespace()->getMembers().count(el.getID())) {
            dynamic_cast<Property&>(el).getNamespace()->getMembers().remove(dynamic_cast<Property&>(el));
        }
    }
}

void Classifier::CheckGeneralizationFunctor::operator()(Element& el) const {
    if (dynamic_cast<Generalization&>(el).getGeneral()) {
        if (dynamic_cast<Generalization&>(el).getGeneral() == m_el) {
            throw InvalidGeneralizationException(el.getIDstring());
        }
    }
}

void Classifier::AddGeneralizationFunctor::operator()(Element& el) const {
    if (!m_el->getRelationships().count(el.getID())) {
        m_el->getRelationships().add(dynamic_cast<Relationship&>(el));
    }

    if (dynamic_cast<Generalization&>(el).getSpecific()) {
        if (dynamic_cast<Generalization&>(el).getSpecific() == m_el) {
            if (dynamic_cast<Generalization&>(el).getGeneral()) {
                if (!dynamic_cast<Classifier*>(m_el)->getGenerals().count(dynamic_cast<Generalization&>(el).getGeneral()->getID())) {
                    dynamic_cast<Classifier*>(m_el)->getGenerals().add(*dynamic_cast<Generalization&>(el).getGeneral());
                }
            }
        }
    } else {
        dynamic_cast<Generalization&>(el).setSpecific(dynamic_cast<Classifier*>(m_el));
    }
}

void Classifier::AddGeneralFunctor::operator()(Element& el) const {
    bool foundGeneralization = false;
    for (auto const& general : dynamic_cast<Classifier*>(m_el)->getGeneralizations()) {
        if (general->getTargets().count(el.getID())) {
            foundGeneralization = true;
        }
    }

    if (!foundGeneralization) {
        Generalization* newGen = new Generalization;
        newGen->setGeneral(&dynamic_cast<Classifier&>(el));
        newGen->setSpecific(dynamic_cast<Classifier*>(m_el));
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

Sequence<Generalization>& Classifier::getGeneralizations() {
    return *m_generalizations;
}

Sequence<Classifier>& Classifier::getGenerals() {
    return *m_generals;
}

bool Classifier::isPrimitive() {
    return false;
}

ElementType Classifier::getElementType() {
    return ElementType::CLASSIFIER;
}