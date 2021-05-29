#ifndef ACTION_H
#define ACTION_H

#include "activityNode.h"
#include "inputPin.h"
#include "outputPin.h"
#include "sequence.h"

namespace UML {
    class Action : public ActivityNode {
        protected:
            Sequence<InputPin> m_inputs;
            Sequence<OutputPin> m_outputs;
            class AddPinFunctor : public AbstractSequenceFunctor {
                public:
                    AddPinFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
        public:
            Action();
            ~Action();
            Sequence<InputPin>& getInputs();
            Sequence<OutputPin>& getOutputs();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
            static ElementType elementType() {
                return ElementType::ACTION;
            };
    };
}

#endif