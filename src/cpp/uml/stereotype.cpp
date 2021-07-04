#include "uml/stereotype.h"

using namespace UML;

Stereotype::Stereotype() {

}

Stereotype::Stereotype(const Stereotype& stereotype) {

}

Stereotype::~Stereotype() {

}

ElementType Stereotype::getElementType() const {
    return ElementType::STEREOTYPE;
}

bool Stereotype::isSubClassOf(ElementType eType) const {
    bool ret = Class::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::STEREOTYPE;
    }

    return ret;
}