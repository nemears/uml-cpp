#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "valueSpecification.h"
#include "orderedSet.h"

namespace UML {
    class Expression : public ValueSpecification {

        friend class UmlManager;

        protected:
            std::string m_symbol = "";
            OrderedSet<ValueSpecification, Expression> m_operands = OrderedSet<ValueSpecification, Expression>(this);
            Set<ValueSpecification, Expression>& getOperandsSet();
            void init();
            void copy(const Expression& rhs);
            Expression();
        public:
            Expression(const Expression& rhs);
            virtual ~Expression();
            OrderedSet<ValueSpecification, Expression>& getOperands();
            std::string getSymbol();
            void setSymbol(std::string sym);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::EXPRESSION;
            };
    };
}
#endif