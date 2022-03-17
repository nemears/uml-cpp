#include "uml/centralBufferNode.h"
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

CentralBufferNode::CentralBufferNode() : Element(ElementType::CENTRAL_BUFFER_NODE) {

}

CentralBufferNode::~CentralBufferNode() {

}

bool CentralBufferNode::isSubClassOf(ElementType eType) const {
    bool ret = ObjectNode::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::CENTRAL_BUFFER_NODE;
    }

    return ret;
}