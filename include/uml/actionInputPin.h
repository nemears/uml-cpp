#ifndef _UML_ACTION_INPUT_PIN_H_
#define _UML_ACTION_INPUT_PIN_H_

#include "inputPin.h"
#include "action.h"

namespace UML {

    typedef UmlPtr<Action> ActionPtr;

    class ActionInputPin : public InputPin {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            Singleton<Action, ActionInputPin> m_fromAction = Singleton<Action, ActionInputPin>(this);
            Set<Action, ActionInputPin>& getFromActionSingleton();
            void init();
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

#endif