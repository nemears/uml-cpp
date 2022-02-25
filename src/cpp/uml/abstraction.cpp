#include "uml/abstraction.h"
#include "uml/uml-stable.h"

using namespace UML;

Abstraction::Abstraction() : Element(ElementType::ABSTRACTION) {

}

Abstraction::Abstraction(const Abstraction& rhs) : Element(rhs, ElementType::ABSTRACTION) {
    init();
    Element::copy(rhs);
    Relationship::copy(rhs);
    DirectedRelationship::copy(rhs);
    NamedElement::copy(rhs);
    ParameterableElement::copy(rhs);
    PackageableElement::copy(rhs);
    Dependency::copy(rhs);
    if (!m_copiedElementFlag) {
        delete &rhs;
    }
}

Abstraction::~Abstraction() {
    mountAndRelease();
}

bool Abstraction::isSubClassOf(ElementType eType) const {
    bool ret = Dependency::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ABSTRACTION;
    }

    return ret;
}