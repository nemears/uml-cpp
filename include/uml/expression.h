#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "valueSpecification.h"

namespace UML {
    class Expression : public ValueSpecification {

        friend class UmlManager;

        protected:
            std::string m_symbol = "";
            Sequence<ValueSpecification> m_operands = Sequence<ValueSpecification>(this);
            class AddOperandFunctor : public TemplateAbstractSequenceFunctor<ValueSpecification,Expression> {
                public:
                    AddOperandFunctor(Expression* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(ValueSpecification& el) const override;
                    void operator()(ID id) const override;
            };
            class RemoveOperandFunctor : public TemplateAbstractSequenceFunctor<ValueSpecification, Expression> {
                public:
                    RemoveOperandFunctor(Expression* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(ValueSpecification& el) const override;
            };
            void setManager(UmlManager* manager) override;
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            Expression();
        public:
            Expression(const Expression& rhs);
            virtual ~Expression();
            Sequence<ValueSpecification>& getOperands();
            std::string getSymbol();
            void setSymbol(std::string sym);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::EXPRESSION;
            };
    };
}
#endif