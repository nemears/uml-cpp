#include "uml/activityGroup.h"
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

Set<Activity, ActivityGroup>& ActivityGroup::getInActivitySingleton() {
    return m_inActivity;
}

Set<ActivityGroup, ActivityGroup>& ActivityGroup::getSuperGroupSingleton() {
    return m_superGroup;
}

void ActivityGroup::referencingReleased(ID id) {
    NamedElement::referencingReleased(id);
    m_containedNodes.release(id);
    m_containedEdges.release(id);
}

void ActivityGroup::referenceReindexed(ID oldID, ID newID) {
    NamedElement::referenceReindexed(oldID, newID);
    m_containedNodes.reindex(oldID, newID);
    m_containedEdges.reindex(oldID, newID);
}

void ActivityGroup::referenceErased(ID id) {
    NamedElement::referenceErased(id);
    m_containedNodes.eraseElement(id);
    m_containedEdges.eraseElement(id);
}

void ActivityGroup::reindexName(ID id, std::string newName) {
    NamedElement::reindexName(id, newName);
    m_containedNodes.reindexName(id, newName);
    m_containedEdges.reindexName(id, newName);
}

void ActivityGroup::init() {
    m_inActivity.subsets(*m_owner);
    m_inActivity.opposite(&Activity::getGroups);
    m_inActivity.m_signature = &ActivityGroup::getInActivitySingleton;
    m_superGroup.subsets(*m_owner);
    m_superGroup.opposite(&ActivityGroup::getSubGroups);
    m_superGroup.m_signature = &ActivityGroup::getSuperGroupSingleton;
    m_superGroup.m_readOnly = true;
    m_containedNodes.opposite(&ActivityNode::getInGroups);
    m_containedNodes.m_signature = &ActivityGroup::getContainedNodes;
    m_containedNodes.m_readOnly = true;
    m_containedEdges.opposite(&ActivityEdge::getInGroups);
    m_containedEdges.m_signature = &ActivityGroup::getContainedEdges;
    m_containedEdges.m_readOnly = true;
    m_subGroups.subsets(*m_ownedElements);
    m_subGroups.opposite(&ActivityGroup::getSuperGroupSingleton);
    m_subGroups.m_signature = &ActivityGroup::getSubGroups;
    m_subGroups.m_readOnly = true;
}

ActivityGroup::ActivityGroup() : Element(ElementType::ACTIVITY_GROUP) {
    init();
}

ActivityGroup::~ActivityGroup() {

}

ActivityPtr ActivityGroup::getInActivity() const {
    return m_inActivity.get();
}

void ActivityGroup::setInActivity(Activity* inActivity) {
    m_inActivity.set(inActivity);
}

void ActivityGroup::setInActivity(Activity& inActivity) {
    m_inActivity.set(inActivity);
}

void ActivityGroup::setInActivity(ActivityPtr inActivity) {
    m_inActivity.set(inActivity);
}

void ActivityGroup::setInActivity(ID id) {
    m_inActivity.set(id);
}

ActivityGroupPtr ActivityGroup::getSuperGroup() const {
    return m_superGroup.get();
}

Set<ActivityNode, ActivityGroup>& ActivityGroup::getContainedNodes() {
    return m_containedNodes;
}

Set<ActivityEdge, ActivityGroup>& ActivityGroup::getContainedEdges() {
    return m_containedEdges;
}

Set<ActivityGroup, ActivityGroup>& ActivityGroup::getSubGroups() {
    return m_subGroups;
}

bool ActivityGroup::isSubClassOf(ElementType eType) const {
    bool ret = NamedElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ACTIVITY_GROUP;
    }

    return ret;
}