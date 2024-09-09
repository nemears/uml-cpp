#include "uml/uml-stable.h"

using namespace UML;

LiteralString::LiteralString(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    LiteralSpecification(elementType, manager)
{}

std::string LiteralString::getValue() {
    return m_value;
}

void LiteralString::setValue(const std::string& val) {
    m_value = val;
}
