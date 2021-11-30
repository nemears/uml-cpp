#ifndef PARAMETERH
#define PARAMETERH

#include "multiplicityElement.h"
#include "connectableElement.h"

namespace UML {

    class Operation;
    namespace Parsers {
        class SetOperation;
        class SetBehavior;
    }

    enum class ParameterDirectionKind {
        IN, 
        INOUT, 
        OUT, 
        RETURN,
        NONE};

    class Parameter : public ConnectableElement , public MultiplicityElement {

        friend class UmlManager;
        friend class Operation;
        friend class Parsers::SetOperation;
        friend class Parsers::SetBehavior;

        protected:
            ParameterDirectionKind m_direction = ParameterDirectionKind::NONE;
            Singleton<Operation, Parameter> m_operation = Singleton<Operation, Parameter>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Set<Operation, Parameter>& getOperationSingleton();
            void init();
            void copy(const Parameter& rhs);
            Parameter();
        public:
            Parameter(const Parameter& rhs);
            virtual ~Parameter();
            Parameter& operator=(Parameter&&) {
                return *this;
            };
            Operation* getOperation();
            Operation& getOperationRef();
            ID getOperationID() const;
            bool hasOperation() const;
            void setOperation(Operation& operation);
            void setOperation(Operation* operation);
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