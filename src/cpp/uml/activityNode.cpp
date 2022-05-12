#include "uml/activityNode.h"
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
#include "uml/interruptibleActivityRegion.h"
#include "uml/umlPtr.h"

using namespace UML;

Set<Activity, ActivityNode>& ActivityNode::getActivitySingleton() {
    return m_activity;
}

void ActivityNode::referencingReleased(ID id) {
    NamedElement::referencingReleased(id);
    RedefinableElement::referencingReleased(id);
    m_incoming.release(id);
    m_outgoing.release(id);
    m_inGroups.release(id);
}

void ActivityNode::referenceReindexed(ID oldID, ID newID) {
    NamedElement::referenceReindexed(oldID, newID);
    RedefinableElement::referenceReindexed(oldID, newID);
    m_incoming.reindex(oldID, newID);
    m_outgoing.reindex(oldID, newID);
    m_inGroups.reindex(oldID, newID);
}

void ActivityNode::referenceErased(ID id) {
    NamedElement::referenceErased(id);
    RedefinableElement::referenceErased(id);
    m_incoming.eraseElement(id);
    m_outgoing.eraseElement(id);
    m_inGroups.eraseElement(id);
}

void ActivityNode::reindexName(ID id, std::string newName) {
    NamedElement::reindexName(id, newName);
    RedefinableElement::reindexName(id, newName);
    m_incoming.reindexName(id, newName);
    m_outgoing.reindexName(id, newName);
    m_inGroups.reindexName(id, newName);
}

void ActivityNode::init() {
    m_activity.subsets(*m_owner);
    m_activity.opposite(&Activity::getNodes);
    m_incoming.opposite(&ActivityEdge::getTargetSingleton);
    m_outgoing.opposite(&ActivityEdge::getSourceSingleton);
    m_inGroups.opposite(&ActivityGroup::getContainedNodes);
    m_inGroups.m_readOnly = true;
    m_inPartitions.subsets(m_inGroups);
    m_inPartitions.opposite(&ActivityPartition::getNodes);
    m_interruptibleRegions.subsets(m_inGroups);
    m_interruptibleRegions.opposite(&InterruptibleActivityRegion::getNodes);
}

ActivityNode::ActivityNode() : Element(ElementType::ACTIVITY_NODE) {
    init();
}

ActivityNode::~ActivityNode() {
    
}

Set<ActivityEdge, ActivityNode>& ActivityNode::getIncoming() {
    return m_incoming;
}

Set<ActivityEdge, ActivityNode>& ActivityNode::getOutgoing() {
    return m_outgoing;
}

ActivityPtr ActivityNode::getActivity() const {
    return m_activity.get();
}

void ActivityNode::setActivity(Activity* activity) {
    m_activity.set(activity);
}

void ActivityNode::setActivity(Activity& activity) {
    m_activity.set(activity);
}

void ActivityNode::setActivity(ActivityPtr activity) {
    m_activity.set(activity);
}

void ActivityNode::setActivity(ID id) {
    m_activity.set(id);
}

Set<ActivityGroup, ActivityNode>& ActivityNode::getInGroups() {
    return m_inGroups;
}

Set<ActivityPartition, ActivityNode>& ActivityNode::getInPartitions() {
    return m_inPartitions;
}

Set<InterruptibleActivityRegion, ActivityNode>& ActivityNode::getInterruptibleRegions() {
    return m_interruptibleRegions;
}

bool ActivityNode::isSubClassOf(ElementType eType) const {
    bool ret = RedefinableElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ACTIVITY_NODE;
    }

    return ret;
}