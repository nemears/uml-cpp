#pragma once

#include "inputPin.h"
#include "action.h"

namespace UML {

    typedef UmlPtr<Action> ActionPtr;

    class ActionInputPin : public InputPin {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            Singleton<Action, ActionInputPin> m_fromAction = Singleton<Action, ActionInputPin>(this);
            Singleton<Action, ActionInputPin>& getFromActionSingleton();
            ActionInputPin();
        public:
            virtual ~ActionInputPin();
            ActionPtr getFromAction() const;
            void setFromAction(Action* action);
            void setFromAction(Action& action);
            void setFromAction(ActionPtr action);
            void setFromAction(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ACTION_INPUT_PIN;
            };
    };
}
