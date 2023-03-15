#include "uml/types/forkNode.h"
#include "uml/types/activity.h"
#include "uml/types/activityEdge.h"
#include "uml/types/package.h"
#include "uml/types/property.h"
#include "uml/types/generalization.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/interruptibleActivityRegion.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

ForkNode::ForkNode() : Element(ElementType::FORK_NODE) {

}

ForkNode::~ForkNode() {

}

bool ForkNode::isSubClassOf(ElementType eType) const {
    bool ret = ControlNode::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::FORK_NODE;
    }

    return ret;
}