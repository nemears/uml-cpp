#ifndef ACTION_H
#define ACTION_H

#include "activityNode.h"
#include "inputPin.h"
#include "outputPin.h"
#include "sequence.h"

namespace UML {
    class Action : public ActivityNode {
        protected:
            Sequence<InputPin>* m_inputs;
            Sequence<OutputPin>* m_outputs;
        public:
            Action();
            ~Action();
            Sequence<InputPin>& getInputs();
            Sequence<OutputPin>& getOutputs();
            ElementType getElementType() override;
    };
}

#endif