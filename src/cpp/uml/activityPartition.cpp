#include "uml/activityPartition.h"
#include "uml/activity.h"
#include "uml/package.h"
#include "uml/property.h"
#include "uml/generalization.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"
#include "uml/interruptibleActivityRegion.h"
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

void ActivityPartition::referenceReindexed(ID oldID, ID newID) {
    ActivityGroup::referenceReindexed(oldID, newID);
    m_represents.reindex(newID);
}

void ActivityPartition::referenceErased(ID id) {
    ActivityGroup::referenceErased(id);
    m_represents.eraseElement(id);
}

void ActivityPartition::init() {
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

ActivityPartition::ActivityPartition() : Element(ElementType::ACTIVITY_PARTITION) {
    init();
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