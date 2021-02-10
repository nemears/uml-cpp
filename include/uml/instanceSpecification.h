#ifndef INSTANCESPECIFICATIONH
#define INSTANCESPECIFICATIONH
#include "namedElement.h"
#include "slot.h"
#include "classifier.h"

namespace UML{
    class InstanceSpecification : public NamedElement {
        public:
            list<Slot*> slots;
            Classifier* getClassifier();
            void setClassifier(Classifier* classifier);
            InstanceSpecification() {
                classifier = NULL;
            }
            ElementType getElementType() override;
            void addSlot(Slot& slot);
            void removeSlot(Slot& slot);
        protected:
            Classifier* classifier;
    };
}

#endif