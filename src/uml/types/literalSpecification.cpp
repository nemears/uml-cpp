#include "uml/uml-stable.h"

using namespace UML;

LiteralSpecification::LiteralSpecification(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    ValueSpecification(elementType, manager)
{}
