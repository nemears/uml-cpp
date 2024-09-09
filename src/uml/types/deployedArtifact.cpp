#include "uml/uml-stable.h"

using namespace UML;

DeployedArtifact::DeployedArtifact(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager)
{}
