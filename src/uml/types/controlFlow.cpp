#include "uml/types/controlFlow.h"
#include "uml/types/activityNode.h"
#include "uml/types/activity.h"
#include "uml/types/package.h"
#include "uml/types/property.h"
#include "uml/types/generalization.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"
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