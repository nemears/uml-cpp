#include "uml/types/activityGroup.h"
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

TypedSet<Activity, ActivityGroup>& ActivityGroup::getInActivitySingleton() {
    return m_inActivity;
}

TypedSet<ActivityGroup, ActivityGroup>& ActivityGroup::getSuperGroupSingleton() {
    return m_superGroup;
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
    m_containedNodes.opposite(&ActivityNode::getInGroups);
    m_containedEdges.opposite(&ActivityEdge::getInGroups);
    m_subGroups.subsets(*m_ownedElements);
    m_subGroups.opposite(&ActivityGroup::getSuperGroupSingleton);
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

ReadOnlySet<ActivityNode, ActivityGroup>& ActivityGroup::getContainedNodes() {
    return m_containedNodes;
}

ReadOnlySet<ActivityEdge, ActivityGroup>& ActivityGroup::getContainedEdges() {
    return m_containedEdges;
}

ReadOnlySet<ActivityGroup, ActivityGroup>& ActivityGroup::getSubGroups() {
    return m_subGroups;
}

bool ActivityGroup::isSubClassOf(ElementType eType) const {
    bool ret = NamedElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ACTIVITY_GROUP;
    }

    return ret;
}