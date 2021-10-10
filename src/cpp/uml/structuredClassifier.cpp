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

    updateCopiedSequenceAddedTo(el, &StructuredClassifier::getOwnedAttributes);
}

void StructuredClassifier::AddOwnedAttributeFunctor::operator()(ID id) const {
    if (!m_el->getAttributes().count(id)) {
        m_el->getAttributes().addByID(id);
    }
    if (!m_el->getRole().count(id)) {
        m_el->getRole().addByID(id);
    }
    if (!m_el->getOwnedMembers().count(id)) {
        m_el->getOwnedMembers().addByID(id);
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
    updateCopiedSequenceRemovedFrom(el, &StructuredClassifier::getOwnedAttributes);
}

void StructuredClassifier::AddRoleFunctor::operator()(ConnectableElement& el) const {
    if (!m_el->getMembers().count(el.getID())) {
        m_el->getMembers().add(el);
    }
    updateCopiedSequenceAddedTo(el, &StructuredClassifier::getRole);
}

void StructuredClassifier::AddRoleFunctor::operator()(ID id) const {
    if (!m_el->getMembers().count(id)) {
        m_el->getMembers().addByID(id);
    }
}

void StructuredClassifier::RemoveRoleFunctor::operator()(ConnectableElement& el) const {
    if (m_el->getMembers().count(el.getID())) {
        m_el->getMembers().remove(el);
    }
    updateCopiedSequenceRemovedFrom(el, &StructuredClassifier::getRole);
}

void StructuredClassifier::AddPartFunctor::operator()(Property& el) const {
    updateCopiedSequenceAddedTo(el, &StructuredClassifier::getParts);
}

void StructuredClassifier::RemovePartFunctor::operator()(Property& el) const {
    updateCopiedSequenceRemovedFrom(el, &StructuredClassifier::getParts);
}

void StructuredClassifier::setManager(UmlManager* manager) {
    Classifier::setManager(manager);
    m_ownedAttributes.m_manager = manager;
    m_role.m_manager = manager;
    m_parts.m_manager = manager;
}

void StructuredClassifier::referenceReindexed(ID oldID, ID newID) {
    Classifier::referenceReindexed(oldID, newID);
    if (m_ownedAttributes.count(oldID)) {
        m_ownedAttributes.reindex(oldID, newID, &StructuredClassifier::getOwnedAttributes);
    }
    if (m_role.count(oldID)) {
        m_role.reindex(oldID, newID, &StructuredClassifier::getRole);
    }
    if (m_parts.count(oldID)) {
        m_parts.reindex(oldID, newID, &StructuredClassifier::getParts);
    }
}

void StructuredClassifier::restoreReferences() {
    Classifier::restoreReferences();
    m_ownedAttributes.restoreReferences();
    m_role.restoreReferences();
    m_parts.restoreReferences();
    // Load all attributes in
    for (auto& attribute : m_ownedAttributes) {
        if (attribute.isComposite()) {
            if (!m_parts.count(attribute.getID())) {
                m_parts.add(attribute);
            }
        }
    }
}

StructuredClassifier::StructuredClassifier() {
    m_ownedAttributes.addProcedures.push_back(new AddOwnedAttributeFunctor(this));
    m_ownedAttributes.removeProcedures.push_back(new RemoveOwnedAttributeFunctor(this));
    m_role.addProcedures.push_back(new AddRoleFunctor(this));
    m_role.removeProcedures.push_back(new RemoveRoleFunctor(this));
    m_parts.addProcedures.push_back(new AddPartFunctor(this));
    m_parts.removeProcedures.push_back(new RemovePartFunctor(this));
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
    m_parts.addProcedures.clear();
    m_parts.removeProcedures.clear();
    m_parts.addProcedures.push_back(new AddPartFunctor(this));
    m_parts.removeProcedures.push_back(new RemovePartFunctor(this));
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

void StructuredClassifier::referencingReleased(ID id) {
    Classifier::referencingReleased(id);
    m_ownedAttributes.elementReleased(id, &StructuredClassifier::getOwnedAttributes);
    m_role.elementReleased(id, &StructuredClassifier::getRole);
    m_parts.elementReleased(id, &StructuredClassifier::getParts);
}