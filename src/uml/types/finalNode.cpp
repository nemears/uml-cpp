#include "uml/types/finalNode.h"
#include "uml/types/activity.h"
#include "uml/types/activityEdge.h"
#include "uml/types/package.h"
#include "uml/types/property.h"
#include "uml/types/generalization.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"
#include "uml/types/interruptibleActivityRegion.h"
#include "uml/umlPtr.h"

using namespace UML;

FinalNode::FinalNode() : Element(ElementType::FINAL_NODE) {

}

FinalNode::~FinalNode() {

}

bool FinalNode::isSubClassOf(ElementType eType) const {
    bool ret = ControlNode::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::FINAL_NODE;
    }

    return ret;
}