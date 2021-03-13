#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "valueSpecification.h"
#include "sequence.h"

namespace UML {
    class Expression : public ValueSpecification {
        protected:
            string m_symbol;
            Sequence<ValueSpecification>* m_operands;
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