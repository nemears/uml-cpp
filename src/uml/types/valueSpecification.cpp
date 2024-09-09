#include "uml/types/namedElement.h"
#include "uml/types/packageableElement.h"
#include "uml/types/parameterableElement.h"
#include "uml/types/typedElement.h"
#include "uml/uml-stable.h"

using namespace UML;

ValueSpecification::ValueSpecification(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    TypedElement(elementType, manager),
    PackageableElement(elementType, manager)
{}
