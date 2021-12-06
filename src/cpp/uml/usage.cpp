#include "uml/usage.h"
#include "uml/uml-stable.h"

using namespace UML;

Usage::Usage() : Element(ElementType::USAGE) {

};

Usage::Usage(const Usage& rhs) : Element(rhs, ElementType::USAGE) {
    init();
    Element::copy(rhs);
    Relationship::copy(rhs);
    DirectedRelationship::copy(rhs);
    NamedElement::copy(rhs);
    ParameterableElement::copy(rhs);
    PackageableElement::copy(rhs);
    Dependency::copy(rhs);
}

Usage::~Usage() {

}

bool Usage::isSubClassOf(ElementType eType) const {
    bool ret = Dependency::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::USAGE;
    }

    return ret;
}