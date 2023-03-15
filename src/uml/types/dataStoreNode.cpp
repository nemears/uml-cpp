#include "uml/types/dataStoreNode.h"
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

DataStoreNode::DataStoreNode() : Element(ElementType::DATA_STORE_NODE) {

}

DataStoreNode::~DataStoreNode() {

}

bool DataStoreNode::isSubClassOf(ElementType eType) const {
    bool ret = CentralBufferNode::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::DATA_STORE_NODE;
    }

    return ret;
}