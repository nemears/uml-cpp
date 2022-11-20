#ifndef _UML_EXPRESSION_H_
#define _UML_EXPRESSION_H_

#include "valueSpecification.h"
#include "set/orderedSet.h"

namespace UML {
    class Expression : public ValueSpecification {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            std::string m_symbol = "";
            CustomOrderedSet<ValueSpecification, Expression> m_operands = CustomOrderedSet<ValueSpecification, Expression>(this);
            void init();
            Expression();
        public:
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