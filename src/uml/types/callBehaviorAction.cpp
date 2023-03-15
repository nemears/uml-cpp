#include "uml/types/callBehaviorAction.h"
#include "uml/types/exceptionHandler.h"
#include "uml/types/activity.h"
#include "uml/types/activityEdge.h"
#include "uml/types/package.h"
#include "uml/types/property.h"
#include "uml/types/generalization.h"
#include "uml/types/interruptibleActivityRegion.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"
#include "uml/types/objectNode.h"
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