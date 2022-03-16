#ifndef _UML_ACTION_H_
#define _UML_ACTION_H_

#include "activityNode.h"
#include "inputPin.h"
#include "outputPin.h"

namespace UML {

    class InputPin;
    class OutputPin;

    class Action : public ActivityNode {

        friend class UmlManager;

        protected:
            Set<InputPin, Action> m_inputs = Set<InputPin, Action>(this);
            Set<OutputPin, Action> m_outputs = Set<OutputPin, Action>(this);
            void init();
            Action();
        public:
            virtual ~Action();
            Set<InputPin, Action>& getInputs();
            Set<OutputPin, Action>& getOutputs();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ACTION;
            };
    };
}

#endif