#include "uml/structuredClassifier.h"
#include "uml/property.h"

using namespace UML;

void StructuredClassifier::AddOwnedAttributeFunctor::operator()(Property& el) const {
    if (!m_el->getAttributes().count(el.getID())) {
        m_el->getAttributes().add(el);
    }

    if (el.getStructuredClassifier() != m_el) {
        el.setStructuredClassifier(m_el);
    }

    if (!m_el->getRole().count(el.getID())) {
        m_el->getRole().add(el);
    }

    if (!m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().add(el);
    }

    // Determine if part and assign
    if (el.isComposite()) {
        m_el->getParts().add(el);
    }
}

void StructuredClassifier::RemoveOwnedAttributeFunctor::operator()(Property& el) const {
    if (el.getStructuredClassifier() == m_el) {
        el.setStructuredClassifier(0);
    }

    if (m_el->getAttributes().count(el.getID())) {
        m_el->getAttributes().remove(el);
    }

    if (m_el->getRole().count(el.getID())) {
        m_el->getRole().remove(el);
    }

    if (m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().remove(el);
    }

    if (m_el->getParts().count(el.getID())) {
        m_el->getParts().remove(el);
    }
}

void StructuredClassifier::AddRoleFunctor::operator()(ConnectableElement& el) const {
    if (!m_el->getMembers().count(el.getID())) {
        m_el->getMembers().add(el);
    }
}

void StructuredClassifier::RemoveRoleFunctor::operator()(ConnectableElement& el) const {
    if (m_el->getMembers().count(el.getID())) {
        m_el->getMembers().remove(el);
    }
}

void StructuredClassifier::setManager(UmlManager* manager) {
    Classifier::setManager(manager);
    m_ownedAttributes.m_manager = manager;
    m_role.m_manager = manager;
    m_parts.m_manager = manager;
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
    m_ownedAttributes.m_el = this;
    m_ownedAttributes.removeProcedures.clear();
    m_ownedAttributes.removeProcedures.push_back(new RemoveOwnedAttributeFunctor(this));
    m_role = clazz.m_role;
    m_role.addProcedures.clear();
    m_role.addProcedures.push_back(new AddRoleFunctor(this));
    m_role.removeProcedures.clear();
    m_role.removeProcedures.push_back(new RemoveRoleFunctor(this));
    m_role.m_el = this;
    m_parts = clazz.m_parts;
    m_parts.m_el = this;
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

bool StructuredClassifier::isSubClassOf(ElementType eType) const {
    bool ret = Classifier::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::STRUCTURED_CLASSIFIER;
    }

    return ret;
}

void StructuredClassifier::restoreReleased(ID id, Element* released) {
    Classifier::restoreReleased(id, released);
    if (m_ownedAttributes.count(id)) {
        released->as<Property>().setStructuredClassifier(this);
    }
}

void StructuredClassifier::referencingReleased(ID id) {
    Classifier::referencingReleased(id);
    m_ownedAttributes.elementReleased(id, &StructuredClassifier::getOwnedAttributes);
    m_role.elementReleased(id, &StructuredClassifier::getRole);
    m_parts.elementReleased(id, &StructuredClassifier::getParts);
}