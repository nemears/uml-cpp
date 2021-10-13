#ifndef VALUE_SPECIFICATION_H
#define VALUE_SPECIFICATION_H

#include "typedElement.h"
#include "sequence.h"
#include "packageableElement.h"

namespace UML {

    class OpaqueBehavior;
    class Property;
    class Expression;
    namespace Parsers {
        class SetOwningSlot;
        class SetOwningInstanceSpec;
        class SetExpression;
    }

    class ValueSpecification : public TypedElement , public PackageableElement {

        friend class UmlManager;
        friend class OpaqueBehavior;
        friend class Property;
        friend class Parsers::SetOwningSlot;
        friend class Parsers::SetOwningInstanceSpec;
        friend class Parsers::SetExpression;

        protected:
            Singleton<Slot, ValueSpecification> m_owningSlot = Singleton<Slot, ValueSpecification>(this);
            class RemoveOwningSlotProcedure : public AbstractSingletonProcedure<Slot, ValueSpecification> {
                public:
                    RemoveOwningSlotProcedure(ValueSpecification* me) : AbstractSingletonProcedure<Slot, ValueSpecification>(me) {};
                    void operator()(Slot* el) const override;
            };
            class AddOwningSlotProcedure : public AbstractSingletonProcedure<Slot, ValueSpecification> {
                public:
                    AddOwningSlotProcedure(ValueSpecification* me) : AbstractSingletonProcedure<Slot, ValueSpecification>(me) {};
                    void operator()(Slot* el) const override;
                    void operator()(ID id) const override;
            };
            Singleton<InstanceSpecification, ValueSpecification> m_owningInstanceSpec = Singleton<InstanceSpecification, ValueSpecification>(this);
            class RemoveOwningInstanceSpecProcedure : public AbstractSingletonProcedure<InstanceSpecification, ValueSpecification> {
                public:
                    RemoveOwningInstanceSpecProcedure(ValueSpecification* me) : AbstractSingletonProcedure<InstanceSpecification, ValueSpecification>(me) {};
                    void operator()(InstanceSpecification* el) const override;
            };
            class AddOwningInstanceSpecProcedure : public AbstractSingletonProcedure<InstanceSpecification, ValueSpecification> {
                public:
                    AddOwningInstanceSpecProcedure(ValueSpecification* me) : AbstractSingletonProcedure<InstanceSpecification, ValueSpecification>(me) {};
                    void operator()(InstanceSpecification* el) const override;
                    void operator()(ID id) const override;
            };
            Singleton<Expression, ValueSpecification> m_expression = Singleton<Expression, ValueSpecification>(this);
            class RemoveExpressionProcedure : public AbstractSingletonProcedure<Expression, ValueSpecification> {
                public:
                    RemoveExpressionProcedure(ValueSpecification* me) : AbstractSingletonProcedure<Expression, ValueSpecification>(me) {};
                    void operator()(Expression* el) const override;
            };
            class AddExpressionProcedure : public AbstractSingletonProcedure<Expression, ValueSpecification> {
                public:
                    AddExpressionProcedure(ValueSpecification* me) : AbstractSingletonProcedure<Expression, ValueSpecification>(me) {};
                    void operator()(Expression* el) const override;
                    void operator()(ID id) const override;
            };
            void reindexName(std::string oldName, std::string newName) override;
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            ValueSpecification();
        public:
            ValueSpecification(const ValueSpecification& rhs);
            Slot* getOwningSlot();
            Slot& getOwningSlotRef();
            ID getOwningSlotID() const;
            bool hasOwningSlot() const;
            void setOwningSlot(Slot* slot);
            void setOwningSlot(Slot& slot);
            // TODO move to protected
            InstanceSpecification* getOwningInstanceSpec();
            InstanceSpecification& getOwningInstanceSpecRef();
            ID getOwningInstanceSpecID() const;
            bool hasOwningInstanceSpec() const;
            void setOwningInstanceSpec(InstanceSpecification* instanceSpec);
            void setOwningInstanceSpec(InstanceSpecification& instanceSpec);
            Expression* getExpression();
            Expression& getExpressionRef();
            ID getExpressionID() const;
            bool hasExpression() const;
            void setExpression(Expression* expression);
            void setExpression(Expression& expression);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::VALUE_SPECIFICATION;
            };
    };
}
#endif