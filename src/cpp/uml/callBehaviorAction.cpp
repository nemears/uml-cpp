#include "uml/callBehaviorAction.h"

using namespace UML;

ElementType CallBehaviorAction::getElementType() {
    return ElementType::CALL_BEHAVIOR_ACTION;
}

Behavior* CallBehaviorAction::getBehavior() {
    return behavior;
}

void CallBehaviorAction::setBehavior(Behavior* bhv) {
    behavior = bhv;
}