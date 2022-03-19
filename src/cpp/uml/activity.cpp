#include "uml/activity.h"
#include "uml/activityEdge.h"
#include "uml/activityNode.h"
#include "uml/activityGroup.h"
#include "uml/behavioralFeature.h"
#include "uml/dataType.h"
#include "uml/operation.h"
#include "uml/parameter.h"
#include "uml/association.h"
#include "uml/interruptibleActivityRegion.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

void Activity::init() {
    m_nodes.subsets(*m_ownedElements);
    m_nodes.opposite(&ActivityNode::getActivitySingleton);
    m_nodes.m_signature = &Activity::getNodes;
    m_edges.subsets(*m_ownedElements);
    m_edges.opposite(&ActivityEdge::getActivitySingleton);
    m_edges.m_signature = &Activity::getEdges;
    m_groups.subsets(*m_ownedElements);
    m_groups.opposite(&ActivityGroup::getInActivitySingleton);
    m_groups.m_signature = &Activity::getGroups;
    m_partitions.subsets(m_groups);
    m_partitions.opposite(&ActivityPartition::getActivitySingleton);
    m_partitions.m_signature = &Activity::getPartitions;
}

Activity::Activity() : Element(ElementType::ACTIVITY) {
    init();
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