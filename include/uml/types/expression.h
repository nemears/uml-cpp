#pragma once

#include "uml/managers/abstractManager.h"
#include "uml/types/element.h"
#include "valueSpecification.h"
#include"uml/set/indexableSet.h"

namespace UML {
    class Expression : public ValueSpecification {

        friend struct ElementInfo<Expression>;

        protected:
            std::string m_symbol = "";
            IndexableOrderedSet<ValueSpecification, Expression> m_operands = IndexableOrderedSet<ValueSpecification, Expression>(this);
        public:
            Expression(std::size_t elementType, AbstractManager& manager);
            IndexableOrderedSet<ValueSpecification, Expression>& getOperands();
            std::string getSymbol();
            void setSymbol(const std::string& sym);
            typedef TypeInfo<std::tuple<ValueSpecification>, Expression> Info;
    };

    template <>
    struct ElementInfo<Expression> : public DefaultInfo {
        static const bool abstract = false;
        inline static const std::string name {"Expression"};
        static SetList sets(Expression& el) {
            return SetList {
                makeSetPair("operands", el.m_operands)
            };
        }
    };
}
