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

TypedSet<Activity, ActivityGroup>& ActivityGroup::getInActivitySingleton() {
    return m_inActivity;
}

TypedSet<ActivityGroup, ActivityGroup>& ActivityGroup::getSuperGroupSingleton() {
    return m_superGroup;
}

void ActivityGroup::referenceReindexed(ID newID) {
    NamedElement::referenceReindexed(newID);
    m_containedNodes.reindex(newID);
    m_containedEdges.reindex(newID);
}

void ActivityGroup::referenceErased(ID id) {
    NamedElement::referenceErased(id);
    m_containedNodes.eraseElement(id);
    m_containedEdges.eraseElement(id);
}

ActivityGroup::ActivityGroup() : Element(ElementType::ACTIVITY_GROUP) {
    m_inActivity.subsets(*m_owner);
    m_inActivity.opposite(&Activity::getGroups);
    m_superGroup.subsets(*m_owner);
    m_superGroup.opposite(&ActivityGroup::getSubGroups);
    m_superGroup.m_readOnly = true;
    m_containedNodes.opposite(&ActivityNode::getInGroups);
    m_containedNodes.m_readOnly = true;
    m_containedEdges.opposite(&ActivityEdge::getInGroups);
    m_containedEdges.m_readOnly = true;
    m_subGroups.subsets(*m_ownedElements);
    m_subGroups.opposite(&ActivityGroup::getSuperGroupSingleton);
    m_subGroups.m_readOnly = true;
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