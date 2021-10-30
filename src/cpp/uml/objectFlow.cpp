#include "uml/objectFlow.h"

using namespace UML;

ObjectFlow::ObjectFlow() : Element(ElementType::OBJECT_FLOW) {}

ObjectFlow::ObjectFlow(const ObjectFlow& rhs) : Element(rhs, ElementType::OBJECT_FLOW) {}

bool ObjectFlow::isSubClassOf(ElementType eType) const {
    bool ret = ActivityEdge::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::OBJECT_FLOW;
    }

    return ret;
}