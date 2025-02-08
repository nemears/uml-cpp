#pragma once

#include "egm/egm-basic.h"

namespace UML {
    template <class>
    class LiteralSpecification;

    template <class ManagerPolicy>
    class LiteralUnlimitedNatural : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<LiteralUnlimitedNatural, EGM::TemplateTypeList<LiteralSpecification>>;
        protected:
            bool m_isInfinite = false;
            int m_number = 0;
        private:
            void init() {}
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(LiteralUnlimitedNatural);
            bool isInfinite() { return m_isInfinite; }
            void setInfinite() { m_isInfinite = true; }
            int getNumber() { return m_number; }
            void setNumber(int n) {
                m_isInfinite = false;
                m_number = n;
            }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::LiteralUnlimitedNatural> {
        static std::string name() { return "LiteralUnlimitedNatural"; }
        template <class Policy>
        struct UnlimitedNaturalPolicy : public EGM::AbstractDataPolicy {
            ManagedPtr<UML::LiteralUnlimitedNatural<Policy>> el;
            UnlimitedNaturalPolicy(UML::LiteralUnlimitedNatural<Policy>& ref) : el(&ref) {}
            std::string getData() override {
                if (el->isInfinite()) {
                    return "*";
                } else {
                    return std::to_string(el->getNumber());
                }
            }
            void setData(std::string data) override {
                if (data == "*") {
                    el->setInfinite();
                } else {
                    el->setNumber(std::stoi(data));
                }
            }
        };
        template <class Policy>
        static DataList data(UML::LiteralUnlimitedNatural<Policy>& el) {
            return DataList {
                createDataPair<UnlimitedNaturalPolicy<Policy>>("value", el)
            };
        }
    };
}
