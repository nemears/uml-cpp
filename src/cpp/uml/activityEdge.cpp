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
#include "uml/interruptibleActivityRegion.h"
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

Set<ValueSpecification, ActivityEdge>& ActivityEdge::getWeightSingleton() {
    return m_weight;
}

Set<InterruptibleActivityRegion, ActivityEdge>& ActivityEdge::getInterruptsSingleton() {
    return m_interrupts;
}

void ActivityEdge::referencingReleased(ID id) {
    NamedElement::referencingReleased(id);
    RedefinableElement::referencingReleased(id);
    m_source.release(id);
    m_target.release(id);
    m_inGroups.release(id);
}

void ActivityEdge::referenceReindexed(ID oldID, ID newID) {
    NamedElement::referenceReindexed(oldID, newID);
    RedefinableElement::referenceReindexed(oldID, newID);
    m_source.reindex(oldID, newID);
    m_target.reindex(oldID, newID);
    m_inGroups.reindex(oldID, newID);
}

void ActivityEdge::referenceErased(ID id) {
    NamedElement::referenceErased(id);
    RedefinableElement::referenceErased(id);
    m_source.eraseElement(id);
    m_target.eraseElement(id);
    m_inGroups.eraseElement(id);
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
    m_weight.subsets(*m_ownedElements);
    m_weight.m_signature = &ActivityEdge::getWeightSingleton;
    m_inGroups.opposite(&ActivityGroup::getContainedEdges);
    m_inGroups.m_signature = &ActivityEdge::getInGroups;
    m_inGroups.m_readOnly = true;
    m_inPartitions.subsets(m_inGroups);
    m_inPartitions.opposite(&ActivityPartition::getEdges);
    m_inPartitions.m_signature = &ActivityEdge::getInPartitions;
    m_interrupts.subsets(m_inGroups);
    m_interrupts.opposite(&InterruptibleActivityRegion::getInterruptingEdges);
    m_interrupts.m_signature = &ActivityEdge::getInterruptsSingleton;
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

Set<ActivityGroup, ActivityEdge>& ActivityEdge::getInGroups() {
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