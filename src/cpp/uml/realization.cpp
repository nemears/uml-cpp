#include "uml/realization.h"

using namespace UML;

Realization::Realization() : Element(ElementType::REALIZATION) {

}

Realization::Realization(const Realization& realization) : Element(realization, ElementType::REALIZATION) {

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