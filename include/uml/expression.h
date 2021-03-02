#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "valueSpecification.h"

namespace UML {
    class Expression : public ValueSpecification {
        protected:
            string symbol;
        public:
            list<ValueSpecification*> operands;
            string getSymbol();
            void setSymbol(string sym);
            ElementType getElementType() override;
            Expression() {
                symbol = "";
            };
    };
}
#endif