#pragma once

#include "egm/egm-basic.h"

namespace UML {
    template <class>
    class LiteralSpecification;

    template <class ManagerPolicy>
    class LiteralInteger : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<LiteralInteger, EGM::TemplateTypeList<LiteralSpecification>>;
        protected:
            int m_value = 0;
        private:
            void init() {}
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(LiteralInteger);
            int getValue() { return m_value; }
            void setValue(int val) { m_value = val; }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::LiteralInteger> {
        static std::string name() { return "LiteralInteger"; }
        template <class Policy>
        struct IntValuePolicy : public EGM::AbstractDataPolicy {
            ManagedPtr<UML::LiteralInteger<Policy>> el;
            IntValuePolicy(UML::LiteralInteger<Policy>& ref) : el(&ref) {}
            std::string getData() override {
                return std::to_string(el->getValue());
            }
            void setData(std::string data) override {
                el->setValue(std::stoi(data));
            }
        };
        template <class Policy>
        static DataList data(UML::LiteralInteger<Policy>& el) {
            return DataList {
                createDataPair<IntValuePolicy<Policy>>("value", el)
            };
        }
    };
}
