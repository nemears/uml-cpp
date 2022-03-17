#include "uml/flowFinalNode.h"
#include "uml/activity.h"
#include "uml/activityEdge.h"
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

FlowFinalNode::FlowFinalNode() : Element(ElementType::FLOW_FINAL_NODE) {

}

FlowFinalNode::~FlowFinalNode() {

}

bool FlowFinalNode::isSubClassOf(ElementType eType) const {
    bool ret = FinalNode::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::FLOW_FINAL_NODE;
    }

    return ret;
}