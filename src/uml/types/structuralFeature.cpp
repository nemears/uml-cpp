#include "uml/types/multiplicityElement.h"
#include "uml/types/namedElement.h"
#include "uml/types/redefinableElement.h"
#include "uml/types/typedElement.h"
#include "uml/uml-stable.h"

using namespace UML;

StructuralFeature::StructuralFeature(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    TypedElement(elementType, manager),
    RedefinableElement(elementType, manager),
    MultiplicityElement(elementType, manager),
    Feature(elementType, manager)
{}

bool StructuralFeature::isReadOnly() const {
    return m_readOnly;
}

void StructuralFeature::setReadOnly(bool readOnly) {
    m_readOnly = readOnly;
}
