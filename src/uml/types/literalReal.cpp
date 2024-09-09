#include "uml/uml-stable.h"

using namespace UML;

LiteralReal::LiteralReal(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    LiteralSpecification(elementType, manager)
{}

double LiteralReal::getValue() {
    return m_value;
}

void LiteralReal::setValue(double val) {
    m_value = val;
}
