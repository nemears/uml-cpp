#include "uml/realization.h"
#include "uml/uml-stable.h"

using namespace UML;

Realization::Realization() : Element(ElementType::REALIZATION) {

}

Realization::Realization(const Realization& rhs) : Element(rhs, ElementType::REALIZATION) {
    init();
    Element::copy(rhs);
    Relationship::copy(rhs);
    DirectedRelationship::copy(rhs);
    NamedElement::copy(rhs);
    ParameterableElement::copy(rhs);
    PackageableElement::copy(rhs);
    Dependency::copy(rhs);
}

Realization::~Realization() {

}

bool Realization::isSubClassOf(ElementType eType) const {
    bool ret = Abstraction::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::REALIZATION;
    }

    return ret;
}