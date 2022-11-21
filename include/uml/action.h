#ifndef _UML_ACTION_H_
#define _UML_ACTION_H_

#include "executableNode.h"
#include "inputPin.h"
#include "outputPin.h"
#include "constraint.h"

namespace UML {

    class InputPin;
    class OutputPin;

    class Action : public ExecutableNode {
        protected:
            CustomSet<InputPin, Action> m_inputs = CustomSet<InputPin, Action>(this);
            CustomSet<OutputPin, Action> m_outputs = CustomSet<OutputPin, Action>(this);
            CustomSet<Constraint, Action> m_localPreconditions = CustomSet<Constraint, Action>(this);
            CustomSet<Constraint, Action> m_localPostconditions = CustomSet<Constraint, Action>(this);
            // TODO context
            bool m_isLocallyReentrant = false;
            void init();
            Action();
        public:
            virtual ~Action();
            bool isLocallyReentrant() const;
            void setIsLocallyReentrant(bool val);
            Set<InputPin, Action>& getInputs();
            Set<OutputPin, Action>& getOutputs();
            Set<Constraint, Action>& getLocalPreconditions();
            Set<Constraint, Action>& getLocalPostconditions();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ACTION;
            };
    };
}

#endif