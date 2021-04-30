#include "uml/callBehaviorAction.h"

using namespace UML;

ElementType CallBehaviorAction::getElementType() const {
    return ElementType::CALL_BEHAVIOR_ACTION;
}

Behavior* CallBehaviorAction::getBehavior() {
    return behavior;
}

void CallBehaviorAction::setBehavior(Behavior* bhv) {
    behavior = bhv;
}

bool CallBehaviorAction::isSubClassOf(ElementType eType) {
    bool ret = Action::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::CALL_BEHAVIOR_ACTION;
    }

    return ret;
}