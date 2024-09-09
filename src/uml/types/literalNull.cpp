#include "uml/uml-stable.h"

using namespace UML;

LiteralNull::LiteralNull(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    LiteralSpecification(elementType, manager)
{}
