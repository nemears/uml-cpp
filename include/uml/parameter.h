#ifndef _UML_PARAMETER_H_
#define _UML_PARAMETER_H_

#include "multiplicityElement.h"
#include "connectableElement.h"

namespace UML {

    class Operation;
    typedef UmlPtr<Operation> OperationPtr;
    namespace Parsers {
        class SetOperation;
        class SetBehavior;
    }

    enum class ParameterDirectionKind {
        IN_UML, 
        INOUT, 
        OUT_UML, 
        RETURN,
        NONE
    };

    class Parameter : public ConnectableElement , public MultiplicityElement {

        friend class UmlManager;
        friend class Operation;

        protected:
            ParameterDirectionKind m_direction = ParameterDirectionKind::NONE;
            Singleton<Operation, Parameter> m_operation = Singleton<Operation, Parameter>(this);
            Set<Operation, Parameter>& getOperationSingleton();
            void init();
            Parameter();
        public:
            virtual ~Parameter();
            OperationPtr getOperation() const;
            void setOperation(Operation& operation);
            void setOperation(Operation* operation);
            void setOperation(ID id);
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