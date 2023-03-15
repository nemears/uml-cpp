#include "uml/types/activityPartition.h"
#include "uml/types/activity.h"
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

TypedSet<Element, ActivityPartition>& ActivityPartition::getRepresentsSingleton() {
    return m_represents;
}

TypedSet<Activity, ActivityPartition>& ActivityPartition::getInActivitySingleton() {
    return m_partitionInActivity;
}

TypedSet<ActivityPartition, ActivityPartition>& ActivityPartition::getSuperPartitionSingleton() {
    return m_superPartition;
}

void ActivityPartition::referenceReindexed(ID newID) {
    ActivityGroup::referenceReindexed(newID);
    m_represents.reindex(newID);
}

void ActivityPartition::referenceErased(ID id) {
    ActivityGroup::referenceErased(id);
    m_represents.eraseElement(id);
}

ActivityPartition::ActivityPartition() : Element(ElementType::ACTIVITY_PARTITION) {
    m_partitionInActivity.redefines(m_inActivity);
    m_partitionInActivity.opposite(&Activity::getPartitions);
    m_superPartition.subsets(m_superGroup);
    m_superPartition.opposite(&ActivityPartition::getSubPartitions);
    m_nodes.subsets(m_containedNodes);
    m_nodes.opposite(&ActivityNode::getInPartitions);
    m_edges.subsets(m_containedEdges);
    m_edges.opposite(&ActivityEdge::getInPartitions);
    m_subPartitions.subsets(m_subGroups);
    m_subPartitions.opposite(&ActivityPartition::getSuperPartitionSingleton);
}

ActivityPartition::~ActivityPartition() {

}

ActivityPartitionPtr ActivityPartition::getSuperPartition() const {
    return m_superPartition.get();
}

void ActivityPartition::setSuperPartition(ActivityPartition* partition) {
    m_superPartition.set(partition);
}

void ActivityPartition::setSuperPartition(ActivityPartition& partition) {
    m_superPartition.set(partition);
}

void ActivityPartition::setSuperPartition(ActivityPartitionPtr partition) {
    m_superPartition.set(partition);
}

void ActivityPartition::setSuperPartition(ID id) {
    m_superPartition.set(id);
}

ElementPtr ActivityPartition::getRepresents() const {
    return m_represents.get();
}

void ActivityPartition::setRepresents(Element* represents) {
    m_represents.set(represents);
}

void ActivityPartition::setRepresents(Element& represents) {
    m_represents.set(represents);
}

void ActivityPartition::setRepresents(ElementPtr represents) {
    m_represents.set(represents);
}

void ActivityPartition::setRepresents(ID id) {
    m_represents.set(id);
}

Set<ActivityNode, ActivityPartition>& ActivityPartition::getNodes() {
    return m_nodes;
}

Set<ActivityEdge, ActivityPartition>& ActivityPartition::getEdges() {
    return m_edges;
}

Set<ActivityPartition, ActivityPartition>& ActivityPartition::getSubPartitions() {
    return m_subPartitions;
}

bool ActivityPartition::isSubClassOf(ElementType eType) const {
    bool ret = ActivityGroup::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ACTIVITY_PARTITION;
    }
    
    return ret;
}