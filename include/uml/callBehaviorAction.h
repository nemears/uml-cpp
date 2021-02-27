#ifndef CALLBEHAVIORACTION_H
#define CALLBEHAVIORACTION_H

#include "action.h"
#include "behavior.h"

namespace UML {
    class CallBehaviorAction : public Action {
        protected:
            Behavior* behavior;
        public:
            Behavior* getBehavior();
            void setBehavior(Behavior* bhv);
            ElementType getElementType() override;
            CallBehaviorAction() {
                behavior = NULL;
            };
    };
}

#endif