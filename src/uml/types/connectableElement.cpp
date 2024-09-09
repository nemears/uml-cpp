#include "uml/uml-stable.h"

using namespace UML;

ConnectableElement::ConnectableElement(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    TypedElement(elementType, manager),
    ParameterableElement(elementType, manager)
{
    m_ends.opposite(&ConnectorEnd::getRoleSingleton);
}

ReadOnlySet<ConnectorEnd, ConnectableElement>& ConnectableElement::getEnds() {
    return m_ends;
}
