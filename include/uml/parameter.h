#ifndef PARAMETERH
#define PARAMETERH
#include "typedElement.h"
#include "multiplicityElement.h"

namespace UML {

    enum class ParameterDirectionKind {
        IN, 
        INOUT, 
        OUT, 
        RETURN,
        NONE};

    class Parameter : public TypedElement , public MultiplicityElement {
        public:
            ElementType getElementType() override;
            ParameterDirectionKind getDirection();
            void setDirection(ParameterDirectionKind direction);
            Parameter() {
                direction = ParameterDirectionKind::NONE;
            }
            string getDirectionString();
            void setDirectionString(string& directionString);
            class InvalidDirectionException : public exception {
                public:
                    virtual const char* what() const throw() {
                        return "Invalid direction given, options are IN, INOUT, OUT or RETURN";
                    }
            }invalidDirectionException;
        protected:
            ParameterDirectionKind direction;
        
    };
}
#endif