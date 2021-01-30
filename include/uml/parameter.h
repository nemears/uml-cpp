#ifndef PARAMETERH
#define PARAMETERH
#include "typedElement.h"

namespace UML {
    class Parameter : public TypedElement {
        public:
            ElementType getElementType() override;
    };
}
#endif