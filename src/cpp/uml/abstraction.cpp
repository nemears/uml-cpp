#include "uml/abstraction.h"

using namespace UML;

Abstraction::Abstraction() : Element(ElementType::ABSTRACTION) {

}

Abstraction::Abstraction(const Abstraction& abstraction) : Element(abstraction, ElementType::ABSTRACTION) {

}

Abstraction::~Abstraction() {

}

bool Abstraction::isSubClassOf(ElementType eType) const {
    bool ret = Dependency::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ABSTRACTION;
    }

    return ret;
}