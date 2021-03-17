#ifndef SLOTH
#define SLOTH
#include "element.h"
#include "valueSpecification.h"
#include "structuralFeature.h"
#include "sequence.h"

namespace UML {

    class InstanceSpecification;

    class Slot : public Element {
        protected:
            StructuralFeature* m_definingFeature;
            Sequence<ValueSpecification>* m_values;
            InstanceSpecification* m_owningInstance;
        public:
            Slot();
            ~Slot();
            Sequence<ValueSpecification>& getValues();
            StructuralFeature* getDefiningFeature();
            InstanceSpecification* getOwningInstance();
            void setOwningInstance(InstanceSpecification* inst);
            void setDefiningFeature(StructuralFeature* definingFeature);
            ElementType getElementType() override;
            class NullDefiningFeatureException : public exception {
                public:
                    virtual const char* what() const throw() {
                        return "Tried to assign value to slot without setting definingFeature";
                    }
            } nullDefiningFeatureException;
    };
}

#endif