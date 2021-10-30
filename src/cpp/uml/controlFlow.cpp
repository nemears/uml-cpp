#include "uml/controlFlow.h"

using namespace UML;

ControlFlow::ControlFlow() : Element(ElementType::CONTROL_FLOW) {}

ControlFlow::ControlFlow(const ControlFlow& rhs) :
ActivityEdge(rhs),
NamedElement(rhs),
Element(rhs, ElementType::CONTROL_FLOW) {}

bool ControlFlow::isSubClassOf(ElementType eType) const {
    bool ret = ActivityEdge::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::CONTROL_FLOW;
    }

    return ret;
}