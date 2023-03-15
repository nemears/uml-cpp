#include "uml/types/activity.h"
#include "uml/types/activityEdge.h"
#include "uml/types/activityNode.h"
#include "uml/types/activityGroup.h"
#include "uml/types/behavioralFeature.h"
#include "uml/types/dataType.h"
#include "uml/types/operation.h"
#include "uml/types/parameter.h"
#include "uml/types/association.h"
#include "uml/types/interruptibleActivityRegion.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

Activity::Activity() : Element(ElementType::ACTIVITY) {
    m_nodes.subsets(*m_ownedElements);
    m_nodes.opposite(&ActivityNode::getActivitySingleton);
    m_edges.subsets(*m_ownedElements);
    m_edges.opposite(&ActivityEdge::getActivitySingleton);
    m_groups.subsets(*m_ownedElements);
    m_groups.opposite(&ActivityGroup::getInActivitySingleton);
    m_partitions.subsets(m_groups);
    m_partitions.opposite(&ActivityPartition::getInActivitySingleton);
}

Activity::~Activity() {
    
}

Set<ActivityNode, Activity>& Activity::getNodes() {
    return m_nodes;
}

Set<ActivityEdge, Activity>& Activity::getEdges() {
    return m_edges;
}

Set<ActivityGroup, Activity>& Activity::getGroups() {
    return m_groups;
}

Set<ActivityPartition, Activity>& Activity::getPartitions() {
    return m_partitions;
}

bool Activity::isSubClassOf(ElementType eType) const {
    bool ret = Behavior::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ACTIVITY;
    }

    return ret;
}