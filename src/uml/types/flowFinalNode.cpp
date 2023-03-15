#include "uml/types/flowFinalNode.h"
#include "uml/types/activity.h"
#include "uml/types/activityEdge.h"
#include "uml/types/package.h"
#include "uml/types/property.h"
#include "uml/types/generalization.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/interruptibleActivityRegion.h"
#include "uml/types/deployment.h"
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