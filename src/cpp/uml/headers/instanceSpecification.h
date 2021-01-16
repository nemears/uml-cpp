#ifndef INSTANCESPECIFICATION
#define INSTANCESPECIFICATION
#include "namedElement.h"
#include "slot.h"
#include "classifier.h"

namespace UML{
    class InstanceSpecification : public NamedElement {
        public:
            list<Slot*> slots;
            Classifier* getClassifier();
            void setClassifier(Classifier* classifier);
        protected:
            Classifier* classifier;
    };
}

#endif