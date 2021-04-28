#include "uml/structuredClassifier.h"

using namespace UML;

void StructuredClassifier::AddOwnedAttributeFunctor::operator()(Element& el) const {
    if (dynamic_cast<Property&>(el).getStructuredClassifier() != m_el) {
        dynamic_cast<Property&>(el).setStructuredClassifier(dynamic_cast<StructuredClassifier*>(m_el));
    }
    if (!dynamic_cast<StructuredClassifier*>(m_el)->getAttributes().count(el.getID())) {
        dynamic_cast<StructuredClassifier*>(m_el)->getAttributes().add(dynamic_cast<Property&>(el));
    }

    if (!dynamic_cast<StructuredClassifier*>(m_el)->getOwnedMembers().count(el.getID())) {
        dynamic_cast<StructuredClassifier*>(m_el)->getOwnedMembers().add(dynamic_cast<Property&>(el));
    }
}

void StructuredClassifier::RemoveOwnedAttributeFunctor::operator()(Element& el) const {
    if (dynamic_cast<Property&>(el).getStructuredClassifier() == m_el) {
        dynamic_cast<Property&>(el).setStructuredClassifier(0);
    }
    if (dynamic_cast<StructuredClassifier*>(m_el)->getAttributes().count(el.getID())) {
        dynamic_cast<StructuredClassifier*>(m_el)->getAttributes().remove(dynamic_cast<Property&>(el));
    }

    if (dynamic_cast<StructuredClassifier*>(m_el)->getOwnedMembers().count(el.getID())) {
        dynamic_cast<StructuredClassifier*>(m_el)->getOwnedMembers().remove(dynamic_cast<Property&>(el));
    }
}

StructuredClassifier::StructuredClassifier() {
    m_ownedAttributes = new Sequence<Property>;
    m_ownedAttributes->addProcedures.push_back(new AddOwnedAttributeFunctor(this));
    m_ownedAttributes->removeProcedures.push_back(new RemoveOwnedAttributeFunctor(this));
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