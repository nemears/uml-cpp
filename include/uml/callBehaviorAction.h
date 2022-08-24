#ifndef _UML_CALL_BEHAVIOR_ACTION_H_
#define _UML_CALL_BEHAVIOR_ACTION_H_

#include "callAction.h"
#include "behavior.h"

namespace UML {
    class CallBehaviorAction : public CallAction {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            Singleton<Behavior, CallBehaviorAction> m_behavior = Singleton<Behavior, CallBehaviorAction>(this);
            Set<Behavior, CallBehaviorAction>& getBehaviorSingleton();
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(ID id, std::string newName) override;
            void referenceErased(ID id) override;
            void init();
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