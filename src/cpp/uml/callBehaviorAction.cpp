#include "uml/callBehaviorAction.h"

using namespace UML;

Behavior* CallBehaviorAction::getBehavior() {
    return behavior;
}

void CallBehaviorAction::setBehavior(Behavior* bhv) {
    behavior = bhv;
}