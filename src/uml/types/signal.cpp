#include "uml/uml-stable.h"

using namespace UML;

Signal::Signal(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    PackageableElement(elementType, manager),
    Classifier(elementType, manager)
{
    m_ownedAttributes.subsets(m_attributes);
    m_ownedAttributes.subsets(m_ownedMembers);
}

IndexableOrderedSet<Property, Signal>& Signal::getOwnedAttributes() {
    return m_ownedAttributes;
}
