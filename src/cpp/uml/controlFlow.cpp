#include "uml/controlFlow.h"

using namespace UML;

ElementType ControlFlow::getElementType() const {
    return ElementType::CONTROL_FLOW;
}

bool ControlFlow::isSubClassOf(ElementType eType) {
    bool ret = ActivityEdge::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::CONTROL_FLOW;
    }

    return ret;
}