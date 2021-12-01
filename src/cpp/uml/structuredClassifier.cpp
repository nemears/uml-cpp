#include "uml/structuredClassifier.h"
#include "uml/property.h"
#include "uml/uml-stable.h"

using namespace UML;

void StructuredClassifier::referencingReleased(ID id) {
    Classifier::referencingReleased(id);
    m_ownedAttributes.release(id);
    m_roles.release(id);
    m_parts.release(id);
}

void StructuredClassifier::referenceReindexed(ID oldID, ID newID) {
    Classifier::referenceReindexed(oldID, newID);
    m_ownedAttributes.reindex(oldID, newID);
    m_roles.reindex(oldID, newID);
    m_parts.reindex(oldID, newID);
}

void StructuredClassifier::restoreReferences() {
    Classifier::restoreReferences();
    // m_ownedAttributes.restoreReferences();
    // m_role.restoreReferences();
    // m_parts.restoreReferences();
    // // Load all attributes in
    // for (auto& attribute : m_ownedAttributes) {
    //     if (attribute.isComposite()) {
    //         if (!m_parts.count(attribute.getID())) {
    //             m_parts.add(attribute);
    //         }
    //     }
    // }
}

void StructuredClassifier::referenceErased(ID id) {
    Classifier::referenceErased(id);
    m_ownedAttributes.eraseElement(id);
    m_roles.eraseElement(id);
    m_parts.eraseElement(id);
}

void StructuredClassifier::init() {
    m_roles.subsets(m_members);
    m_roles.m_signature = &StructuredClassifier::getRoles;
    m_ownedAttributes.subsets(m_attributes);
    m_ownedAttributes.subsets(m_roles);
    m_ownedAttributes.subsets(m_ownedMembers);
    m_ownedAttributes.m_signature = &StructuredClassifier::getOwnedAttributes;
    m_parts.subsets(m_ownedAttributes);
    m_parts.m_signature = &StructuredClassifier::getParts;
    m_parts.m_readOnly = true;
}

void StructuredClassifier::copy(const StructuredClassifier& rhs) {
    m_roles = rhs.m_roles;
    m_ownedAttributes = rhs.m_ownedAttributes;
    m_parts = rhs.m_parts;
}

StructuredClassifier::StructuredClassifier() : Element(ElementType::STRUCTURED_CLASSIFIER) {
    init();
}

StructuredClassifier::StructuredClassifier(const StructuredClassifier& clazz) : Element(clazz, ElementType::STRUCTURED_CLASSIFIER) {
    // abstract
}

StructuredClassifier::~StructuredClassifier() {
    
}

Set<Property, StructuredClassifier>& StructuredClassifier::getOwnedAttributes() {
    return m_ownedAttributes;
}

Set<ConnectableElement, StructuredClassifier>& StructuredClassifier::getRoles() {
    return m_roles;
}

Set<Property, StructuredClassifier>& StructuredClassifier::getParts() {
    return m_parts;
}

bool StructuredClassifier::isSubClassOf(ElementType eType) const {
    bool ret = Classifier::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::STRUCTURED_CLASSIFIER;
    }

    return ret;
}