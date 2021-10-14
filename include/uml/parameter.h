#ifndef PARAMETERH
#define PARAMETERH

#include "multiplicityElement.h"
#include "sequence.h"
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
        friend class Parsers::SetOperation;
        friend class Parsers::SetBehavior;

        protected:
            ParameterDirectionKind m_direction;
            Singleton<Operation, Parameter> m_operation = Singleton<Operation, Parameter>(this);
            class RemoveOperationProcedure : public AbstractSingletonProcedure<Operation, Parameter> {
                public:
                    RemoveOperationProcedure(Parameter* me) : AbstractSingletonProcedure<Operation, Parameter>(me) {};
                    void operator()(Operation* el) const override;
            };
            class AddOperationProcedure : public AbstractSingletonProcedure<Operation, Parameter> {
                public:
                    AddOperationProcedure(Parameter* me) : AbstractSingletonProcedure<Operation, Parameter>(me) {};
                    void operator()(Operation* el) const override;
                    void operator()(ID id) const override;
            };
            Singleton<Behavior, Parameter> m_behavior = Singleton<Behavior, Parameter>(this);
            class RemoveBehaviorProcedure : public AbstractSingletonProcedure<Behavior, Parameter> {
                public:
                    RemoveBehaviorProcedure(Parameter* me) : AbstractSingletonProcedure<Behavior, Parameter>(me) {};
                    void operator()(Behavior* el) const override;
            };
            class AddBehaviorProcedure : public AbstractSingletonProcedure<Behavior, Parameter> {
                public:
                    AddBehaviorProcedure(Parameter* me) : AbstractSingletonProcedure<Behavior, Parameter>(me) {};
                    void operator()(Behavior* el) const override;
                    void operator()(ID id) const override;
            };
            void reindexName(std::string oldName, std::string newName) override;
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Parameter();
        public:
            Parameter(const Parameter& parameter);
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
            Behavior* getBehavior();
            Behavior& getBehaviorRef();
            ID getBehaviorID() const;
            bool hasBehavior() const;
            void setBehavior(Behavior* behavior);
            void setBehavior(Behavior& behavior);
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