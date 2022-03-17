#include "uml/dataStoreNode.h"
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