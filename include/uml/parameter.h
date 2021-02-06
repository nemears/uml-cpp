#ifndef PARAMETERH
#define PARAMETERH
#include "typedElement.h"

namespace UML {

    enum class ParameterDirectionKind {
        IN, 
        INOUT, 
        OUT, 
        RETURN};

    class Parameter : public TypedElement {
        public:
            ElementType getElementType() override;
            ParameterDirectionKind getDirection();
            void setDirection(ParameterDirectionKind direction);
        protected:
            ParameterDirectionKind direction;
        
    };
}
#endif