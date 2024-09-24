#include "uml/types/structuredClassifier.h"
#include "uml/uml-stable.h"

using namespace UML;

void StructuredClassifier::PartPolicy::elementAdded(Property& el, StructuredClassifier& me) {
    if (el.isComposite() && !me.m_parts.contains(el)) {
        me.m_parts.innerAdd(&el);
    }
}

void StructuredClassifier::PartPolicy::elementRemoved(Property& el, StructuredClassifier& me) {
    if (el.isComposite() && me.m_parts.contains(el)) {
        me.m_parts.innerRemove(&el);
    }
}

StructuredClassifier::StructuredClassifier(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    PackageableElement(elementType, manager),
    Classifier(elementType, manager)
{
    m_roles.subsets(m_members);
    m_ownedAttributes.subsets(m_ownedMembers);
    m_ownedAttributes.subsets(m_attributes);
    m_ownedAttributes.subsets(m_roles);
    m_ownedConnectors.subsets(m_ownedMembers);
    m_ownedConnectors.subsets(m_features);
}

IndexableSet<Property, StructuredClassifier, StructuredClassifier::PartPolicy>& StructuredClassifier::getOwnedAttributes() {
    return m_ownedAttributes;
}

ReadOnlyIndexableSet<ConnectableElement, StructuredClassifier>& StructuredClassifier::getRoles() {
    return m_roles;
}

ReadOnlyIndexableSet<Property, StructuredClassifier>& StructuredClassifier::getParts() {
    return m_parts;
}

IndexableSet<Connector, StructuredClassifier>& StructuredClassifier::getOwnedConnectors() {
    return m_ownedConnectors;
}
