#include "uml/joinNode.h"
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

Set<ValueSpecification, JoinNode>& JoinNode::getJoinSpecSingleton() {
    return m_joinSpec;
}

void JoinNode::init() {
    m_joinSpec.subsets(*m_ownedElements);
}

JoinNode::JoinNode() : Element(ElementType::JOIN_NODE) {
    init();
}

JoinNode::~JoinNode() {

}

ValueSpecificationPtr JoinNode::getJoinSpec() const {
    return m_joinSpec.get();
}

void JoinNode::setJoinSpec(ValueSpecification* joinSpec) {
    m_joinSpec.set(joinSpec);
}

void JoinNode::setJoinSpec(ValueSpecification& joinSpec) {
    m_joinSpec.set(joinSpec);
}

void JoinNode::setJoinSpec(ID id) {
    m_joinSpec.set(id);
}

bool JoinNode::isSubClassOf(ElementType eType) const {
    bool ret = ControlNode::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::JOIN_NODE;
    }

    return ret;
}