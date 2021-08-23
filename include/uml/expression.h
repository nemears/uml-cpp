#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "valueSpecification.h"

namespace UML {
    class Expression : public ValueSpecification {
        protected:
            std::string m_symbol;
            Sequence<ValueSpecification> m_operands;
            class AddOperandFunctor : public TemplateAbstractSequenceFunctor<ValueSpecification,Expression> {
                public:
                    AddOperandFunctor(Expression* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(ValueSpecification& el) const override;
            };
        public:
            Sequence<ValueSpecification>& getOperands();
            std::string getSymbol();
            void setSymbol(std::string sym);
            ElementType getElementType() const override;
            Expression();
            ~Expression();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::EXPRESSION;
            };
    };
}
#endif