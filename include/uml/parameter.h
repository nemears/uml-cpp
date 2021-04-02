#ifndef PARAMETERH
#define PARAMETERH
#include "typedElement.h"
#include "multiplicityElement.h"
#include "sequence.h"

namespace UML {

    class Operation;

    enum class ParameterDirectionKind {
        IN, 
        INOUT, 
        OUT, 
        RETURN,
        NONE};

    class Parameter : public TypedElement , public MultiplicityElement {
        protected:
            ParameterDirectionKind m_direction;
            Operation* m_operation;
            void reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) override;
            void reindexName(string oldName, string newName) override;
        public:
            Operation* getOperation();
            void setOperation(Operation* operation);
            ElementType getElementType() override;
            ParameterDirectionKind getDirection();
            void setDirection(ParameterDirectionKind direction);
            Parameter() {
                m_direction = ParameterDirectionKind::NONE;
                m_operation = 0;
            }
            string getDirectionString();
            void setDirectionString(string& directionString);
            class InvalidDirectionException : public exception {
                public:
                    virtual const char* what() const throw() {
                        return "Invalid direction given, options are IN, INOUT, OUT or RETURN";
                    }
            }invalidDirectionException;
            bool isSubClassOf(ElementType eType) override;
    };
}
#endif