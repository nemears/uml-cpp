#include "uml/structuredClassifier.h"
#include "uml/property.h"

using namespace UML;

void StructuredClassifier::referencingReleased(ID id) {
    Classifier::referencingReleased(id);
    m_ownedAttributes.release(id);
    // m_role.elementReleased(id, &StructuredClassifier::getRole);
    // m_parts.elementReleased(id, &StructuredClassifier::getParts);
}

void StructuredClassifier::referenceReindexed(ID oldID, ID newID) {
    Classifier::referenceReindexed(oldID, newID);
    m_ownedAttributes.reindex(oldID, newID);
    // m_role.reindex(oldID, newID, &StructuredClassifier::getRole);
    // m_parts.reindex(oldID, newID, &StructuredClassifier::getParts);
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
    // m_role.elementErased(id);
    // m_parts.elementErased(id);
}

void StructuredClassifier::init() {
    m_ownedAttributes.subsets(m_attributes);
    m_ownedAttributes.subsets(m_ownedMembers);
    m_ownedAttributes.m_signature = &StructuredClassifier::getOwnedAttributes;
}

void StructuredClassifier::copy(const StructuredClassifier& rhs) {
    m_ownedAttributes = rhs.m_ownedAttributes;
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

// Sequence<ConnectableElement>& StructuredClassifier::getRole() {
//     return m_role;
// }

// Sequence<Property>& StructuredClassifier::getParts() {
//     return m_parts;
// }

bool StructuredClassifier::isSubClassOf(ElementType eType) const {
    bool ret = Classifier::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::STRUCTURED_CLASSIFIER;
    }

    return ret;
}