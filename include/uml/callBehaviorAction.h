#ifndef CALLBEHAVIORACTION_H
#define CALLBEHAVIORACTION_H

#include "action.h"
#include "behavior.h"

namespace UML {
    class CallBehaviorAction : public Action {
        protected:
            Behavior* behavior;
            CallBehaviorAction();
        public:
            CallBehaviorAction(const CallBehaviorAction& rhs);
            Behavior* getBehavior();
            void setBehavior(Behavior* bhv);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::CALL_BEHAVIOR_ACTION;
            };
    };
}

#endif