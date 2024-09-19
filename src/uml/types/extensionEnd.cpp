#include "uml/managers/abstractManager.h"
#include "uml/types/namedElement.h"
#include "uml/types/parameterableElement.h"
#include "uml/types/property.h"
#include "uml/types/redefinableElement.h"
#include "uml/types/typedElement.h"
#include "uml/uml-stable.h"

using namespace UML;

Singleton<Stereotype, ExtensionEnd>& ExtensionEnd::getTypeSingleton() {
    return m_extensionType;
}

ExtensionEnd::ExtensionEnd(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    TypedElement(elementType, manager),
    RedefinableElement(elementType, manager),
    ParameterableElement(elementType, manager),
    Property(elementType, manager)
{
    m_extensionType.redefines(m_type);
}

StereotypePtr ExtensionEnd::getType() const {
    return m_extensionType.get();
}

void ExtensionEnd::setType(StereotypePtr type) {
    m_extensionType.set(type);
}

void ExtensionEnd::setType(Stereotype& type) {
    m_extensionType.set(type);
}
