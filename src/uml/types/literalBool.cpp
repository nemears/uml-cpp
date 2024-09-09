#include "uml/uml-stable.h"

using namespace UML;

LiteralBool::LiteralBool(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    LiteralSpecification(elementType, manager)
{}

bool LiteralBool::getValue() {
    return m_value;;
}

void LiteralBool::setValue(bool val) {
    m_value = val;
}
