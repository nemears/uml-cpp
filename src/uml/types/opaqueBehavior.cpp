#include "uml/uml-stable.h"

namespace UML {

OpaqueBehavior::OpaqueBehavior(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    PackageableElement(elementType, manager),
    Classifier(elementType, manager),
    Behavior(elementType, manager)
{
    m_bodies.subsets(m_ownedElements);
}
    
OrderedSet<LiteralString, OpaqueBehavior>& OpaqueBehavior::getBodies() {
    return m_bodies;
}
}
