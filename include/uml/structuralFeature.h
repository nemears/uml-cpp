#ifndef STRUCTURALFEATUREH
#define STRUCTURALFEATUREH

#include "typedElement.h"
#include "multiplicityElement.h"
#include "feature.h"

namespace UML {
    class StructuralFeature : public TypedElement, public MultiplicityElement, public Feature {
        public:
            ElementType getElementType() override;
            class InvalidValueException: public exception {
                public:
                    virtual const char* what() const throw() {
                        return "tried to assign value that does not match structural features corresponded type";
                    }
            } invalidValueException;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif