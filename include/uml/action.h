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
            Set<InputPin, Action> m_inputs = Set<InputPin, Action>(this);
            Set<OutputPin, Action> m_outputs = Set<OutputPin, Action>(this);
            Set<Constraint, Action> m_localPreconditions = Set<Constraint, Action>(this);
            Set<Constraint, Action> m_localPostconditions = Set<Constraint, Action>(this);
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