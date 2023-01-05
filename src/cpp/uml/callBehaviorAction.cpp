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

using namespace UML;

TypedSet<Behavior, CallBehaviorAction>& CallBehaviorAction::getBehaviorSingleton() {
    return m_behavior;
}

void CallBehaviorAction::referenceReindexed(ID newID) {
    CallAction::referenceReindexed(newID);
    m_behavior.reindex(newID);
}

void CallBehaviorAction::referenceErased(ID id) {
    CallAction::referenceErased(id);
    m_behavior.eraseElement(id);
}

CallBehaviorAction::CallBehaviorAction() : Element(ElementType::CALL_BEHAVIOR_ACTION) {
    
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