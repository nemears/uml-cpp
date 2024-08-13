#pragma once

#include "valueSpecification.h"
#include"uml/set/indexableSet.h"

namespace UML {
    class Expression : public ValueSpecification {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            std::string m_symbol = "";
            IndexableOrderedSet<ValueSpecification, Expression> m_operands = IndexableOrderedSet<ValueSpecification, Expression>(this);
            Expression();
        public:
            virtual ~Expression();
            IndexableOrderedSet<ValueSpecification, Expression>& getOperands();
            std::string getSymbol();
            void setSymbol(const std::string& sym);
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::EXPRESSION;
            };
    };
}
