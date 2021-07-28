#include "uml/abstraction.h"

using namespace UML;

Abstraction::Abstraction() {

}

Abstraction::Abstraction(const Abstraction& abstraction) {

}

Abstraction::~Abstraction() {

}

ElementType Abstraction::getElementType() const {
    return ElementType::ABSTRACTION;
}

bool Abstraction::isSubClassOf(ElementType eType) const {
    bool ret = Dependency::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ABSTRACTION;
    }

    return ret;
}