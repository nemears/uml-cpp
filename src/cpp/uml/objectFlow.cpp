#include "uml/objectFlow.h"

using namespace UML;

ElementType ObjectFlow::getElementType() const {
    return ElementType::OBJECT_FLOW;
}

bool ObjectFlow::isSubClassOf(ElementType eType) const {
    bool ret = ActivityEdge::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::OBJECT_FLOW;
    }

    return ret;
}