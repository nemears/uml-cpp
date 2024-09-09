#include "uml/uml-stable.h"

using namespace UML;

PrimitiveType::PrimitiveType(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    PackageableElement(elementType, manager),
    DataType(elementType, manager)
{}
