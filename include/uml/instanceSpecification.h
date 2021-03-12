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
        public:
            Classifier* getClassifier();
            void setClassifier(Classifier* classifier);
            Sequence<Slot>& getSlots();
            InstanceSpecification();
            ~InstanceSpecification();
            ElementType getElementType() override;
    };
}

#endif