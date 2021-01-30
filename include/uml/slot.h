#ifndef SLOTH
#define SLOTH
#include "element.h"
#include "valueSpecification.h"
#include "structuralFeature.h"

namespace UML {
    class Slot : public Element {
        public:
            list<ValueSpecification*> values;
            StructuralFeature* getDefiningFeature();
            void setDefiningFeature(StructuralFeature* definingFeature);
            ElementType getElementType() override;
        protected:
            StructuralFeature* definingFeature;
    };
}

#endif