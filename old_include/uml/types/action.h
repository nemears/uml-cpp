#pragma once

#include "executableNode.h"
#include "inputPin.h"
#include "outputPin.h"
#include "constraint.h"

namespace UML {

    class InputPin;
    class OutputPin;

    class Action : public ExecutableNode {
        protected:
            ReadOnlySet<InputPin, Action> m_inputs = ReadOnlySet<InputPin, Action>(this);
            ReadOnlySet<OutputPin, Action> m_outputs = ReadOnlySet<OutputPin, Action>(this);
            Set<Constraint, Action> m_localPreconditions = CustomSet<Constraint, Action>(this);
            Set<Constraint, Action> m_localPostconditions = CustomSet<Constraint, Action>(this);
            // TODO context
            bool m_isLocallyReentrant = false;
            Action();
        public:
            virtual ~Action();
            bool isLocallyReentrant() const;
            void setIsLocallyReentrant(bool val);
            ReadOnlySet<InputPin, Action>& getInputs();
            ReadOnlySet<OutputPin, Action>& getOutputs();
            Set<Constraint, Action>& getLocalPreconditions();
            Set<Constraint, Action>& getLocalPostconditions();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ACTION;
            };
    };
}
