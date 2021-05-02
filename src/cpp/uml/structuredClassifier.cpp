#include "uml/structuredClassifier.h"

using namespace UML;

void StructuredClassifier::AddOwnedAttributeFunctor::operator()(Element& el) const {
    if (dynamic_cast<Property&>(el).getStructuredClassifier() != m_el) {
        dynamic_cast<Property&>(el).setStructuredClassifier(dynamic_cast<StructuredClassifier*>(m_el));
    }

    if (!dynamic_cast<StructuredClassifier*>(m_el)->getAttributes().count(el.getID())) {
        dynamic_cast<StructuredClassifier*>(m_el)->getAttributes().add(dynamic_cast<Property&>(el));
    }

    if (!dynamic_cast<StructuredClassifier*>(m_el)->getRole().count(el.getID())) {
        dynamic_cast<StructuredClassifier*>(m_el)->getRole().add(dynamic_cast<Property&>(el));
    }

    if (!dynamic_cast<StructuredClassifier*>(m_el)->getOwnedMembers().count(el.getID())) {
        dynamic_cast<StructuredClassifier*>(m_el)->getOwnedMembers().add(dynamic_cast<Property&>(el));
    }

    // Determine if part and assign
    if (dynamic_cast<Property&>(el).isComposite()) {
        dynamic_cast<StructuredClassifier*>(m_el)->getParts().add(dynamic_cast<Property&>(el));
    }
}

void StructuredClassifier::RemoveOwnedAttributeFunctor::operator()(Element& el) const {
    if (dynamic_cast<Property&>(el).getStructuredClassifier() == m_el) {
        dynamic_cast<Property&>(el).setStructuredClassifier(0);
    }

    if (dynamic_cast<StructuredClassifier*>(m_el)->getAttributes().count(el.getID())) {
        dynamic_cast<StructuredClassifier*>(m_el)->getAttributes().remove(dynamic_cast<Property&>(el));
    }

    if (dynamic_cast<StructuredClassifier*>(m_el)->getRole().count(el.getID())) {
        dynamic_cast<StructuredClassifier*>(m_el)->getRole().remove(dynamic_cast<Property&>(el));
    }

    if (dynamic_cast<StructuredClassifier*>(m_el)->getOwnedMembers().count(el.getID())) {
        dynamic_cast<StructuredClassifier*>(m_el)->getOwnedMembers().remove(dynamic_cast<Property&>(el));
    }

    if (dynamic_cast<StructuredClassifier*>(m_el)->getParts().count(el.getID())) {
        dynamic_cast<StructuredClassifier*>(m_el)->getParts().remove(dynamic_cast<Property&>(el));
    }
}

void StructuredClassifier::AddRoleFunctor::operator()(Element& el) const {
    if (!dynamic_cast<StructuredClassifier*>(m_el)->getMembers().count(el.getID())) {
        dynamic_cast<StructuredClassifier*>(m_el)->getMembers().add(dynamic_cast<ConnectableElement&>(el));
    }
}

void StructuredClassifier::RemoveRoleFunctor::operator()(Element& el) const {
    if (dynamic_cast<StructuredClassifier*>(m_el)->getMembers().count(el.getID())) {
        dynamic_cast<StructuredClassifier*>(m_el)->getMembers().remove(dynamic_cast<ConnectableElement&>(el));
    }
}

StructuredClassifier::StructuredClassifier() {
    m_ownedAttributes.addProcedures.push_back(new AddOwnedAttributeFunctor(this));
    m_ownedAttributes.removeProcedures.push_back(new RemoveOwnedAttributeFunctor(this));
    m_role.addProcedures.push_back(new AddRoleFunctor(this));
    m_role.removeProcedures.push_back(new RemoveRoleFunctor(this));
}

StructuredClassifier::StructuredClassifier(const StructuredClassifier& clazz) : Classifier(clazz) {
    m_ownedAttributes = clazz.m_ownedAttributes;
    m_ownedAttributes.addProcedures.clear();
    m_ownedAttributes.addProcedures.push_back(new AddOwnedAttributeFunctor(this));
    m_ownedAttributes.removeProcedures.clear();
    m_ownedAttributes.removeProcedures.push_back(new RemoveOwnedAttributeFunctor(this));
    m_role = clazz.m_role;
    m_role.addProcedures.clear();
    m_role.addProcedures.push_back(new AddRoleFunctor(this));
    m_role.removeProcedures.clear();
    m_role.removeProcedures.push_back(new RemoveRoleFunctor(this));
    m_parts = clazz.m_parts;
}

StructuredClassifier::~StructuredClassifier() {
    
}

Sequence<Property>& StructuredClassifier::getOwnedAttributes() {
    return m_ownedAttributes;
}

Sequence<ConnectableElement>& StructuredClassifier::getRole() {
    return m_role;
}

Sequence<Property>& StructuredClassifier::getParts() {
    return m_parts;
}

ElementType StructuredClassifier::getElementType() const {
    return ElementType::STRUCTURED_CLASSIFIER;
}

bool StructuredClassifier::isSubClassOf(ElementType eType) {
    bool ret = Classifier::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::STRUCTURED_CLASSIFIER;
    }

    return ret;
}