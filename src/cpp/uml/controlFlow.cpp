#include "uml/controlFlow.h"
#include "uml/activityNode.h"
#include "uml/activity.h"
#include "uml/package.h"
#include "uml/property.h"
#include "uml/generalization.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

ControlFlow::ControlFlow() : Element(ElementType::CONTROL_FLOW) {

}

ControlFlow::~ControlFlow() {

}

bool ControlFlow::isSubClassOf(ElementType eType) const {
    bool ret = ActivityEdge::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::CONTROL_FLOW;
    }

    return ret;
}