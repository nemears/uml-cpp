#include "uml/activityFinalNode.h"
#include "uml/activity.h"
#include "uml/activityEdge.h"
#include "uml/package.h"
#include "uml/property.h"
#include "uml/generalization.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/interruptibleActivityRegion.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

ActivityFinalNode::ActivityFinalNode() : Element(ElementType::ACTIVITY_FINAL_NODE) {

};

ActivityFinalNode::~ActivityFinalNode() {

}

bool ActivityFinalNode::isSubClassOf(ElementType eType) const  {
    bool ret = FinalNode::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ACTIVITY_FINAL_NODE;
    }

    return ret;
}