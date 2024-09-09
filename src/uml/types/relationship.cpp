#include "uml/uml-stable.h"
using namespace UML;

Relationship::Relationship(std::size_t elementType, AbstractManager& manager) : Element(elementType, manager) {}

ReadOnlySet<Element, Relationship>& Relationship::getRelatedElements() {
    return m_relatedElements;
}
