#include "uml/types/joinNode.h"
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
#include "uml/umlPtr.h"

using namespace UML;

TypedSet<ValueSpecification, JoinNode>& JoinNode::getJoinSpecSingleton() {
    return m_joinSpec;
}

JoinNode::JoinNode() : Element(ElementType::JOIN_NODE) {
    m_joinSpec.subsets(*m_ownedElements);
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