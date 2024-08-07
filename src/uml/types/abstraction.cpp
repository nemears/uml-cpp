#include "uml/uml-stable.h"

using namespace UML;

Abstraction::Abstraction() : Element(ElementType::ABSTRACTION) {

}

Abstraction::~Abstraction() {
    
}

bool Abstraction::is(ElementType eType) const {
    bool ret = Dependency::is(eType);

    if (!ret) {
        ret = eType == ElementType::ABSTRACTION;
    }

    return ret;
}
