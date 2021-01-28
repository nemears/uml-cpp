#ifndef SLOT
#define SLOT
#include "element.h"
#include "valueSpecification.h"
#include "structuralFeature.h"

namespace UML {
    class Slot : public Element {
        public:
            list<ValueSpecification*> values;
            StructuralFeature* getDefiningFeature();
            void setDefiningFeature(StructuralFeature* definingFeature);
        protected:
            StructuralFeature* definingFeature;
    };
}

#endif