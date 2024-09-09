#include "uml/uml-stable.h"

using namespace UML;

Realization::Realization(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    Abstraction(elementType, manager)
{}
