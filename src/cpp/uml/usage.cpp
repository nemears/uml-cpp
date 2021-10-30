#include "uml/usage.h"

using namespace UML;

Usage::Usage() : Element(ElementType::USAGE) {

};

Usage::Usage(const Usage& usage) : 
Dependency(usage),
Element(usage, ElementType::USAGE) {

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