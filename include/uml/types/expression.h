#pragma once

#include "valueSpecification.h"
#include"uml/set/orderedSet.h"

namespace UML {
    class Expression : public ValueSpecification {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            std::string m_symbol = "";
            OrderedSet<ValueSpecification, Expression> m_operands = OrderedSet<ValueSpecification, Expression>(this);
            Expression();
        public:
            virtual ~Expression();
            OrderedSet<ValueSpecification, Expression>& getOperands();
            std::string getSymbol();
            void setSymbol(const std::string& sym);
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::EXPRESSION;
            };
    };
}
