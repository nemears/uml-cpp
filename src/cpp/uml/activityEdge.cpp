#include "uml/activityEdge.h"
#include "uml/activityNode.h"
#include "uml/activity.h"
#include "uml/package.h"
#include "uml/property.h"
#include "uml/generalization.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

Set<Activity, ActivityEdge>& ActivityEdge::getActivitySingleton() {
    return m_activity;
}

Set<ActivityNode, ActivityEdge>& ActivityEdge::getSourceSingleton() {
    return m_source;
}

Set<ActivityNode, ActivityEdge>& ActivityEdge::getTargetSingleton() {
    return m_target;
}

Set<ValueSpecification, ActivityEdge>& ActivityEdge::getGuardSingleton() {
    return m_guard;
}

void ActivityEdge::referencingReleased(ID id) {
    NamedElement::referencingReleased(id);
    RedefinableElement::referencingReleased(id);
    m_source.release(id);
    m_target.release(id);
}

void ActivityEdge::referenceReindexed(ID oldID, ID newID) {
    NamedElement::referenceReindexed(oldID, newID);
    RedefinableElement::referenceReindexed(oldID, newID);
    m_source.reindex(oldID, newID);
    m_target.reindex(oldID, newID);
}

void ActivityEdge::referenceErased(ID id) {
    NamedElement::referenceErased(id);
    RedefinableElement::referenceErased(id);
    m_source.eraseElement(id);
    m_target.eraseElement(id);
}

void ActivityEdge::restoreReference(Element* el) {
    NamedElement::restoreReference(el);
}

void ActivityEdge::init() {
    m_activity.subsets(*m_owner);
    m_activity.opposite(&Activity::getEdges);
    m_activity.m_signature = &ActivityEdge::getActivitySingleton;
    m_source.opposite(&ActivityNode::getOutgoing);
    m_source.m_signature = &ActivityEdge::getSourceSingleton;
    m_target.opposite(&ActivityNode::getIncoming);
    m_target.m_signature = &ActivityEdge::getTargetSingleton;
    m_guard.subsets(*m_ownedElements);
    m_guard.m_signature = &ActivityEdge::getGuardSingleton;
}

ActivityEdge::ActivityEdge() : Element(ElementType::ACTIVITY_EDGE) {
    init();
}

ActivityEdge::~ActivityEdge() {

}

ActivityPtr ActivityEdge::getActivity() const{
    return m_activity.get();
}

void ActivityEdge::setActivity(Activity* activity) {
    m_activity.set(activity);
}

void ActivityEdge::setActivity(Activity& activity) {
    m_activity.set(activity);
}

ActivityNodePtr ActivityEdge::getSource() const {
    return m_source.get();
}

void ActivityEdge::setSource(ActivityNode* source) {
    m_source.set(source);
}

void ActivityEdge::setSource(ActivityNode& source) {
    m_source.set(source);
}

ActivityNodePtr ActivityEdge::getTarget() const {
    return m_target.get();
}

void ActivityEdge::setTarget(ActivityNode* target) {
    m_target.set(target);
}

void ActivityEdge::setTarget(ActivityNode& target) {
    m_target.set(target);
}

ValueSpecificationPtr ActivityEdge::getGuard() {
    return m_guard.get();
}

void ActivityEdge::setGuard(ValueSpecification* guard) {
    m_guard.set(guard);
}

void ActivityEdge::setGuard(ValueSpecification& guard) {
    m_guard.set(guard);
}

bool ActivityEdge::isSubClassOf(ElementType eType) const {
    bool ret = RedefinableElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ACTIVITY_EDGE;
    }

    return ret;
}