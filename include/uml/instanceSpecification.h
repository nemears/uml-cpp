#ifndef INSTANCESPECIFICATIONH
#define INSTANCESPECIFICATIONH
#include "namedElement.h"
#include "slot.h"
#include "classifier.h"

namespace UML{
    class InstanceSpecification : public NamedElement {
        protected:
            Classifier* m_classifier;
            Sequence<Slot>* m_slots;
            class AddSlotFunctor : public AbstractSequenceFunctor {
                public:
                    AddSlotFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveSlotFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveSlotFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
        public:
            Classifier* getClassifier();
            void setClassifier(Classifier* classifier);
            Sequence<Slot>& getSlots();
            InstanceSpecification();
            ~InstanceSpecification();
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif