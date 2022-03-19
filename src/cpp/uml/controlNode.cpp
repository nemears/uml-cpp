#include "uml/controlNode.h"
#include "uml/activity.h"
#include "uml/activityEdge.h"
#include "uml/package.h"
#include "uml/property.h"
#include "uml/generalization.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/interruptibleActivityRegion.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

ControlNode::ControlNode() : Element(ElementType::CONTROL_NODE) {

}

ControlNode::~ControlNode() {

}

bool ControlNode::isSubClassOf(ElementType eType) const {
    bool ret = ActivityNode::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::CONTROL_NODE;
    }

    return ret;
}