#include "uml/uml-stable.h"

using namespace UML;

Profile::Profile(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    Package(elementType, manager) 
{}
