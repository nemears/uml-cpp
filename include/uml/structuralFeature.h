#ifndef STRUCTURALFEATUREH
#define STRUCTURALFEATUREH

#include "typedElement.h"

namespace UML {
    class StructuralFeature : public TypedElement {
        public:
            ElementType getElementType() override;
            class InvalidValueException: public exception {
                virtual const char* what() const throw() {
                    return "tried to assign value that does not match structural features corresponded type";
                }
            } invalidValueException;
    };
}

#endif