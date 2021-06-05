#ifndef INSTANCESPECIFICATIONH
#define INSTANCESPECIFICATIONH

#include "packageableElement.h"
#include "slot.h"
#include "classifier.h"

namespace UML{
    class InstanceSpecification : public PackageableElement {
        protected:
            ID m_classifierID;
            Classifier* m_classifierPtr;
            Sequence<Slot> m_slots;
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
            void setManager(UmlManager* manager) override;
        public:
            InstanceSpecification();
            InstanceSpecification(const InstanceSpecification& inst);
            virtual ~InstanceSpecification();
            Classifier* getClassifier();
            void setClassifier(Classifier* classifier);
            Sequence<Slot>& getSlots();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
            static ElementType elementType() {
                return ElementType::INSTANCE_SPECIFICATION;
            };
    };
}

#endif