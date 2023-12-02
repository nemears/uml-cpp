#include "uml/types/activityEdge.h"
#include "uml/types/activityNode.h"
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

TypedSet<Activity, ActivityEdge>& ActivityEdge::getActivitySingleton() {
    return m_activity;
}

TypedSet<ActivityNode, ActivityEdge>& ActivityEdge::getSourceSingleton() {
    return m_source;
}

TypedSet<ActivityNode, ActivityEdge>& ActivityEdge::getTargetSingleton() {
    return m_target;
}

TypedSet<ValueSpecification, ActivityEdge>& ActivityEdge::getGuardSingleton() {
    return m_guard;
}

TypedSet<ValueSpecification, ActivityEdge>& ActivityEdge::getWeightSingleton() {
    return m_weight;
}

TypedSet<InterruptibleActivityRegion, ActivityEdge>& ActivityEdge::getInterruptsSingleton() {
    return m_interrupts;
}

void ActivityEdge::referenceErased(ID id) {
    NamedElement::referenceErased(id);
    RedefinableElement::referenceErased(id);
    m_source.eraseElement(id);
    m_target.eraseElement(id);
    m_inGroups.eraseElement(id);
}

ActivityEdge::ActivityEdge() : Element(ElementType::ACTIVITY_EDGE) {
    m_activity.subsets(*m_owner);
    m_activity.opposite(&Activity::getEdges);
    m_source.opposite(&ActivityNode::getOutgoing);
    m_target.opposite(&ActivityNode::getIncoming);
    m_guard.subsets(*m_ownedElements);
    m_weight.subsets(*m_ownedElements);
    m_inGroups.opposite(&ActivityGroup::getContainedEdges);
    m_inPartitions.subsets(m_inGroups);
    m_inPartitions.opposite(&ActivityPartition::getEdges);
    m_interrupts.subsets(m_inGroups);
    m_interrupts.opposite(&InterruptibleActivityRegion::getInterruptingEdges);
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

void ActivityEdge::setActivity(ActivityPtr activity) {
    m_activity.set(activity);
}

void ActivityEdge::setActivity(ID id) {
    m_activity.set(id);
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

void ActivityEdge::setSource(ActivityNodePtr source) {
    m_source.set(source);
}

void ActivityEdge::setSource(ID id) {
    m_source.set(id);
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

void ActivityEdge::setTarget(ActivityNodePtr target) {
    m_target.set(target);
}

void ActivityEdge::setTarget(ID id) {
    m_target.set(id);
}

ValueSpecificationPtr ActivityEdge::getGuard() const {
    return m_guard.get();
}

void ActivityEdge::setGuard(ValueSpecification* guard) {
    m_guard.set(guard);
}

void ActivityEdge::setGuard(ValueSpecification& guard) {
    m_guard.set(guard);
}

void ActivityEdge::setGuard(ValueSpecificationPtr guard) {
    m_guard.set(guard);
}

void ActivityEdge::setGuard(ID id) {
    m_guard.set(id);
}

ValueSpecificationPtr ActivityEdge::getWeight() const {
    return m_weight.get();
}

void ActivityEdge::setWeight(ValueSpecification* weight) {
    m_weight.set(weight);
}

void ActivityEdge::setWeight(ValueSpecification& weight) {
    m_weight.set(weight);
}

void ActivityEdge::setWeight(ValueSpecificationPtr weight) {
    m_weight.set(weight);
}

void ActivityEdge::setWeight(ID id) {
    m_weight.set(id);
}

ReadOnlySet<ActivityGroup, ActivityEdge>& ActivityEdge::getInGroups() {
    return m_inGroups;
}

Set<ActivityPartition, ActivityEdge>& ActivityEdge::getInPartitions() {
    return m_inPartitions;
}

InterruptibleActivityRegionPtr ActivityEdge::getInterrupts() const {
    return m_interrupts.get();
}

void ActivityEdge::setInterrupts(InterruptibleActivityRegion* interrupts) {
    m_interrupts.set(interrupts);
}

void ActivityEdge::setInterrupts(InterruptibleActivityRegion& interrupts) {
    m_interrupts.set(interrupts);
}

void ActivityEdge::setInterrupts(InterruptibleActivityRegionPtr interrupts) {
    m_interrupts.set(interrupts);
}

void ActivityEdge::setInterrupts(ID id) {
    m_interrupts.set(id);
}

bool ActivityEdge::isSubClassOf(ElementType eType) const {
    bool ret = RedefinableElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ACTIVITY_EDGE;
    }

    return ret;
}