#pragma once

#include "egm/egm-basic.h"

namespace UML {
    template <class>
    class LiteralSpecification;

    template <class ManagerPolicy>
    class LiteralString : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<LiteralString, EGM::TemplateTypeList<LiteralSpecification>>;
        protected:
            std::string m_value = "";
        private:
            void init() {}
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(LiteralString);
            std::string getValue() { return m_value; }
            void setValue(std::string& val) { m_value = val; }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::LiteralString> {
        static std::string name() { return "LiteralString"; }
        template <class Policy>
        struct StringValuePolicy : public EGM::AbstractDataPolicy {
            ManagedPtr<UML::LiteralString<Policy>> el;
            StringValuePolicy(UML::LiteralString<Policy>& ref) : el(&ref) {}
            std::string getData() override { return el->getValue(); }
            void setData(std::string data) override { el->setValue(data); }
        };
        template <class Policy>
        static DataList data(UML::LiteralString<Policy>& el) {
            return DataList {
                createDataPair<StringValuePolicy<Policy>>("value", el)
            };
        }
    };
}
