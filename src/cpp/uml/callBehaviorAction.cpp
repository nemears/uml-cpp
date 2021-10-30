#include "uml/callBehaviorAction.h"

using namespace UML;

CallBehaviorAction::CallBehaviorAction() : Element(ElementType::CALL_BEHAVIOR_ACTION) {
    /** TODO: this **/
}

CallBehaviorAction::CallBehaviorAction(const CallBehaviorAction& rhs) : 
Action(rhs),
RedefinableElement(rhs),
NamedElement(rhs),
Element(ElementType::CALL_BEHAVIOR_ACTION) {
    /** TODO: this **/
}

Behavior* CallBehaviorAction::getBehavior() {
    return behavior;
}

void CallBehaviorAction::setBehavior(Behavior* bhv) {
    behavior = bhv;
}

bool CallBehaviorAction::isSubClassOf(ElementType eType) const {
    bool ret = Action::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::CALL_BEHAVIOR_ACTION;
    }

    return ret;
}