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
#include "uml/setReferenceFunctor.h"

using namespace UML;

Set<Element, ActivityPartition>& ActivityPartition::getRepresentsSingleton() {
    return m_represents;
}

Set<Activity, ActivityPartition>& ActivityPartition::getInActivitySingleton() {
    return m_partitionInActivity;
}

Set<ActivityPartition, ActivityPartition>& ActivityPartition::getSuperPartitionSingleton() {
    return m_superPartition;
}

void ActivityPartition::referencingReleased(ID id) {
    ActivityGroup::referencingReleased(id);
    m_represents.release(id);
}

void ActivityPartition::referenceReindexed(ID oldID, ID newID) {
    ActivityGroup::referenceReindexed(oldID, newID);
    m_represents.reindex(oldID, newID);
}

void ActivityPartition::referenceErased(ID id) {
    ActivityGroup::referenceErased(id);
    m_represents.eraseElement(id);
}

void ActivityPartition::reindexName(ID id, std::string newName) {
    ActivityGroup::reindexName(id, newName);
    m_represents.reindexName(id, newName);
}

void ActivityPartition::init() {
    m_represents.m_signature = &ActivityPartition::getRepresentsSingleton;
    m_represents.m_addFunctors.insert(new SetReferenceFunctor(this));
    m_represents.m_removeFunctors.insert(new RemoveReferenceFunctor(this));
    m_partitionInActivity.redefines(m_inActivity);
    m_partitionInActivity.opposite(&Activity::getPartitions);
    m_partitionInActivity.m_signature = &ActivityPartition::getInActivitySingleton;
    m_superPartition.subsets(m_superGroup);
    m_superPartition.opposite(&ActivityPartition::getSubPartitions);
    m_superPartition.m_signature = &ActivityPartition::getSuperPartitionSingleton;
    m_nodes.subsets(m_containedNodes);
    m_nodes.opposite(&ActivityNode::getInPartitions);
    m_nodes.m_signature = &ActivityPartition::getNodes;
    m_edges.subsets(m_containedEdges);
    m_edges.opposite(&ActivityEdge::getInPartitions);
    m_edges.m_signature = &ActivityPartition::getEdges;
    m_subPartitions.subsets(m_subGroups);
    m_subPartitions.opposite(&ActivityPartition::getSuperPartitionSingleton);
    m_subPartitions.m_signature = &ActivityPartition::getSubPartitions;
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