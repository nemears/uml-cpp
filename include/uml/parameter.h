#ifndef PARAMETERH
#define PARAMETERH
#include "typedElement.h"

namespace UML {

    enum class ParameterDirectionKind {
        IN, 
        INOUT, 
        OUT, 
        RETURN,
        NONE};

    class Parameter : public TypedElement {
        public:
            ElementType getElementType() override;
            ParameterDirectionKind getDirection();
            void setDirection(ParameterDirectionKind direction);
            Parameter() {
                direction = ParameterDirectionKind::NONE;
            }
        protected:
            ParameterDirectionKind direction;
        
    };
}
#endif