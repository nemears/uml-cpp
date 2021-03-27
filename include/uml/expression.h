#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "valueSpecification.h"
#include "sequence.h"

namespace UML {
    class Expression : public ValueSpecification {
        protected:
            string m_symbol;
            Sequence<ValueSpecification>* m_operands;
            class AddOperandFunctor : public AbstractSequenceFunctor {
                public:
                    AddOperandFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
        public:
            Sequence<ValueSpecification>& getOperands();
            string getSymbol();
            void setSymbol(string sym);
            ElementType getElementType() override;
            Expression();
            ~Expression();
    };
}
#endif