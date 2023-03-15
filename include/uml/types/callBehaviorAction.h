#ifndef _UML_CALL_BEHAVIOR_ACTION_H_
#define _UML_CALL_BEHAVIOR_ACTION_H_

#include "callAction.h"
#include "behavior.h"

namespace UML {
    class CallBehaviorAction : public CallAction {

        template <typename AccessPolicy, typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            CustomSingleton<Behavior, CallBehaviorAction> m_behavior = CustomSingleton<Behavior, CallBehaviorAction>(this);
            TypedSet<Behavior, CallBehaviorAction>& getBehaviorSingleton();
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
            CallBehaviorAction();
        public:
            virtual ~CallBehaviorAction();
            BehaviorPtr getBehavior() const;
            void setBehavior(Behavior* behavior);
            void setBehavior(Behavior& behavior);
            void setBehavior(BehaviorPtr behavior);
            void setBehavior(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::CALL_BEHAVIOR_ACTION;
            };
    };
}

#endif