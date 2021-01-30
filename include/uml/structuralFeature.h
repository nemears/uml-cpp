#ifndef STRUCTURALFEATUREH
#define STRUCTURALFEATUREH

#include "typedElement.h"

namespace UML {
    class StructuralFeature : public TypedElement {
        public:
            ElementType getElementType() override;
    };
}

#endif