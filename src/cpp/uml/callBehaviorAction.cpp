#include "uml/callBehaviorAction.h"
#include "uml/exceptionHandler.h"
#include "uml/activity.h"
#include "uml/activityEdge.h"
#include "uml/package.h"
#include "uml/property.h"
#include "uml/generalization.h"
#include "uml/interruptibleActivityRegion.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"
#include "uml/objectNode.h"
#include "uml/umlPtr.h"
#include "uml/setReferenceFunctor.h"

using namespace UML;

Set<Behavior, CallBehaviorAction>& CallBehaviorAction::getBehaviorSingleton() {
    return m_behavior;
}

void CallBehaviorAction::referencingReleased(ID id) {
    CallAction::referencingReleased(id);
    m_behavior.release(id);
}

void CallBehaviorAction::referenceReindexed(ID oldID, ID newID) {
    CallAction::referenceReindexed(oldID, newID);
    m_behavior.reindex(oldID, newID);
}

void CallBehaviorAction::reindexName(ID id, std::string newName) {
    CallAction::reindexName(id, newName);
    m_behavior.reindexName(id, newName);
}

void CallBehaviorAction::referenceErased(ID id) {
    CallAction::referenceErased(id);
    m_behavior.eraseElement(id);
}

void CallBehaviorAction::init() {
    m_behavior.m_addFunctors.insert(new SetReferenceFunctor(this));
    m_behavior.m_removeFunctors.insert(new RemoveReferenceFunctor(this));
}

CallBehaviorAction::CallBehaviorAction() : Element(ElementType::CALL_BEHAVIOR_ACTION) {
    init();
}

CallBehaviorAction::~CallBehaviorAction() {

}

BehaviorPtr CallBehaviorAction::getBehavior() const {
    return m_behavior.get();
}

void CallBehaviorAction::setBehavior(Behavior* behavior) {
    m_behavior.set(behavior);
}

void CallBehaviorAction::setBehavior(Behavior& behavior) {
    m_behavior.set(behavior);
}

void CallBehaviorAction::setBehavior(BehaviorPtr behavior) {
    m_behavior.set(behavior);
}

void CallBehaviorAction::setBehavior(ID id) {
    m_behavior.set(id);
}

bool CallBehaviorAction::isSubClassOf(ElementType eType) const {
    bool ret = CallAction::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::CALL_BEHAVIOR_ACTION;
    }

    return ret;
}