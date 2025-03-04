#pragma once

#include "egm/egm-basic.h"

namespace UML {
    template <class>
    class LiteralSpecification;

    template <class ManagerPolicy>
    class LiteralReal : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<LiteralReal, EGM::TemplateTypeList<LiteralSpecification>>;
        protected:
            double m_value = 0;
        private:
            void init() {}
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(LiteralReal);
            double getValue() const { return m_value; }
            void setValue(double value) { m_value = value; }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::LiteralReal> {
        static std::string name() { return "LiteralReal"; }
        template <class Policy>
        struct RealPolicy : public EGM::AbstractDataPolicy {
            ManagedPtr<UML::LiteralReal<Policy>> el;
            RealPolicy(UML::LiteralReal<Policy>& ref) : el(&ref) {}
            std::string getData() override {
                return std::to_string(el->getValue());
            }
            void setData(std::string data) override {
                char* rest {};
                el->setValue(std::strtod(data.c_str(), &rest));
            }
        };
        template <class Policy>
        static DataList data(UML::LiteralReal<Policy>& el) {
            return DataList {
                createDataPair<RealPolicy<Policy>>("value", el)
            };
        }
    };
}
