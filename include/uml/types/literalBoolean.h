#pragma once

#include "egm/egm-basic.h"
#include "uml/util/boolDataPolicy.h"

namespace UML {
    template <class>
    class LiteralSpecification;

    template <class ManagerPolicy>
    class LiteralBoolean : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<LiteralBoolean, EGM::TemplateTypeList<LiteralSpecification>>;
        protected:
            bool m_value = false;
        private:
            void init() {}
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(LiteralBoolean);
            bool getValue() { return m_value; }
            void setValue(bool val) { m_value = val; }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::LiteralBoolean> {
        static std::string name() { return "LiteralBoolean"; }
        template <class Policy>
        struct LiteralBoolPolicy : public UML::BooleanDataPolicy {
            ManagedPtr<UML::LiteralBoolean<Policy>> el;
            LiteralBoolPolicy(UML::LiteralBoolean<Policy>& ref) : el(&ref) {}
            void setBool(bool b) override { el->setValue(b); }
            bool getBool() override { return el->getValue(); }
        };
        template <class Policy>
        static DataList data(UML::LiteralBoolean<Policy>& el) {
            return DataList {
                createDataPair<LiteralBoolPolicy<Policy>>("value", el)
            };
        }
    };
}
