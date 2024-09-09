#include "uml/uml-stable.h"

using namespace UML;

Abstraction::Abstraction(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    Dependency(elementType, manager) 
{}
