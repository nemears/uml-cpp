#include "uml/usage.h"

using namespace UML;

Usage::Usage() {

};

Usage::Usage(const Usage& usage) {

}

Usage::~Usage() {

}

ElementType Usage::getElementType() const {
    return ElementType::USAGE;
}

bool Usage::isSubClassOf(ElementType eType) const {
    bool ret = Dependency::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::USAGE;
    }

    return ret;
}