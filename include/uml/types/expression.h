#pragma once

#include "egm/egm-basic.h"
#include "uml/util/indexableSet.h"

namespace UML {
    template <class>
    class ValueSpecification;

    template <class ManagerPolicy>
    class Expression : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<Expression, EGM::TemplateTypeList<ValueSpecification>>;
        protected:
            using Operands = IndexableOrderedSet<ValueSpecification, Expression>;
            Operands m_operands = Operands(this);
            std::string m_symbol = "";
        private:
            void init() {
                m_operands.subsets(ManagerPolicy::m_ownedElements);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Expression);

            Operands& getOperands() { return m_operands; }
            std::string getSymbol() const { return m_symbol; }
            void setSymbol(std::string symbol) { m_symbol = symbol; }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Expression> {
        static std::string name() { return "Expression"; }
        template <class Policy>
        static SetList sets(UML::Expression<Policy>& el) {
            return SetList {
                make_set_pair("operands", el.getOperands())
            };
        }

        template <class Policy>
        struct SymbolPolicy: AbstractDataPolicy {
            ManagedPtr<UML::Expression<Policy>> el;
            SymbolPolicy(UML::Expression<Policy>& ref) : el(&ref) {}
            std::string getData() { return el->getSymbol(); }
            void setData(std::string data) { el->setSymbol(data); }
        };

        template <class Policy>
        static DataList data(UML::Expression<Policy>& el) {
            return DataList {
                createDataPair<SymbolPolicy<Policy>>("symbol", el)
            };
        }
    };
}
