#ifndef ACTION_H
#define ACTION_H

#include "activityNode.h"
#include "inputPin.h"
#include "outputPin.h"
#include "sequence.h"

namespace UML {
    class Action : public ActivityNode {
        protected:
            Sequence<InputPin> m_inputs = Sequence<InputPin>(this);
            Sequence<OutputPin> m_outputs = Sequence<OutputPin>(this);
            class AddInputFunctor : public TemplateAbstractSequenceFunctor<InputPin,Action> {
                public:
                    AddInputFunctor(Action* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(InputPin& el) const override;
            };
            class RemoveInputFunctor : public TemplateAbstractSequenceFunctor<InputPin, Action> {
                public:
                    RemoveInputFunctor(Action* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(InputPin& el) const override;
            };
            class AddOutputFunctor : public TemplateAbstractSequenceFunctor<OutputPin,Action> {
                public:
                    AddOutputFunctor(Action* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(OutputPin& el) const override;
            };
            class RemoveOutputFunctor : public TemplateAbstractSequenceFunctor<OutputPin, Action> {
                public:
                    RemoveOutputFunctor(Action* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(OutputPin& el) const override;
            };
            void setManager(UmlManager* manager);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            Action();
        public:
            Action(const Action& rhs);
            virtual ~Action();
            Sequence<InputPin>& getInputs();
            Sequence<OutputPin>& getOutputs();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ACTION;
            };
    };
}

#endif