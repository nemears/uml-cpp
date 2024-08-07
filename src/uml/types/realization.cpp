#include "uml/uml-stable.h"

using namespace UML;

Realization::Realization() : Element(ElementType::REALIZATION) {

}

Realization::~Realization() {
    
}

bool Realization::is(ElementType eType) const {
    bool ret = Abstraction::is(eType);

    if (!ret) {
        ret = eType == ElementType::REALIZATION;
    }

    return ret;
}
