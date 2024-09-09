#include "uml/uml-stable.h"

using namespace UML;

LiteralInt::LiteralInt(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    LiteralSpecification(elementType, manager)
{}

int LiteralInt::getValue() {
    return m_value;
}

void LiteralInt::setValue(int val) {
    m_value = val;
}
