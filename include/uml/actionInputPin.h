#ifndef _UML_ACTION_INPUT_PIN_H_
#define _UML_ACTION_INPUT_PIN_H_

#include "inputPin.h"
#include "action.h"

namespace UML {

    typedef UmlPtr<Action> ActionPtr;

    class ActionInputPin : public InputPin {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            CustomSingleton<Action, ActionInputPin> m_fromAction = CustomSingleton<Action, ActionInputPin>(this);
            TypedSet<Action, ActionInputPin>& getFromActionSingleton();
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