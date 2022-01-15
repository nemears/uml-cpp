#include "uml/structuredClassifier.h"
#include "uml/property.h"
#include "uml/uml-stable.h"

using namespace UML;

void StructuredClassifier::AddPartFunctor::operator()(Element& el) const {
    if (el.as<Property>().isComposite() && !m_el.as<StructuredClassifier>().m_parts.contains(el.getID())) {
        m_el.as<StructuredClassifier>().m_parts.nonOppositeAdd(el.as<Property>());
    }
}

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
    for (auto& prop : m_ownedAttributes) {
        if (prop.isComposite() && !m_parts.contains(prop)) {
            m_parts.addReadOnly(prop.getID());
        }
    }
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
    m_roles.m_readOnly = true;
    m_ownedAttributes.subsets(m_ownedMembers);
    m_ownedAttributes.subsets(m_attributes);
    m_ownedAttributes.subsets(m_roles);
    m_ownedAttributes.m_signature = &StructuredClassifier::getOwnedAttributes;
    m_ownedAttributes.m_addFunctors.insert(new AddPartFunctor(this));
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