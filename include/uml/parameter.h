#ifndef PARAMETERH
#define PARAMETERH

#include "multiplicityElement.h"
#include "sequence.h"
#include "connectableElement.h"

namespace UML {

    class Operation;

    enum class ParameterDirectionKind {
        IN, 
        INOUT, 
        OUT, 
        RETURN,
        NONE};

    class Parameter : public ConnectableElement , public MultiplicityElement {
        protected:
            ParameterDirectionKind m_direction;
            ID m_operationID;
            Operation* m_operationPtr;
            void reindexID(ID oldID, ID newID) override;
            void reindexName(std::string oldName, std::string newName) override;
        public:
            Parameter();
            Parameter(const Parameter& parameter);
            virtual ~Parameter();
            Parameter& operator=(Parameter&&) {
                return *this;
            };
            Operation* getOperation();
            void setOperation(Operation* operation);
            ElementType getElementType() const override;
            ParameterDirectionKind getDirection();
            void setDirection(ParameterDirectionKind direction);
            std::string getDirectionString();
            void setDirectionString(std::string& directionString);
            class InvalidDirectionException : public std::exception {
                public:
                    virtual const char* what() const throw() {
                        return "Invalid direction given, options are IN, INOUT, OUT or RETURN";
                    }
            }invalidDirectionException;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PARAMETER;
            };
    };
}
#endif