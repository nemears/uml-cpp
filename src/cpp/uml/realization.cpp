#include "uml/realization.h"

using namespace UML;

Realization::Realization() {

}

Realization::Realization(const Realization& realization) {

}

Realization::~Realization() {

}

ElementType Realization::getElementType() const {
    return ElementType::REALIZATION;
}

bool Realization::isSubClassOf(ElementType eType) const {
    bool ret = Abstraction::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::REALIZATION;
    }

    return ret;
}