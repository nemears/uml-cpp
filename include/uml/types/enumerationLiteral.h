#pragma once

#include "egm/egm-basic.h"

namespace UML {
    template <class>
    class InstanceSpecification;

    template <class>
    class Enumeration;

    template <class ManagerPolicy>
    class EnumerationLiteral : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<EnumerationLiteral, EGM::TemplateTypeList<InstanceSpecification>>;
        protected:
            using EnumerationSingleton = EGM::Singleton<Enumeration, EnumerationLiteral>;
            EnumerationSingleton m_enumeration = EnumerationSingleton(this);
        private:
            void init() {
                m_enumeration.subsets(ManagerPolicy::m_namespace);
                m_enumeration.opposite(&EnumerationSingleton::ManagedType::getOwnedLiterals);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(EnumerationLiteral);
            
            EnumerationSingleton& getEnumerationSingleton() { return m_enumeration; }
            using EnumerationImpl = typename EnumerationSingleton::ManagedType;
            using EnumerationPtr = EGM::ManagedPtr<EnumerationImpl>;
            EnumerationPtr getEnumeration() const  { return m_enumeration.get(); }
            void setEnumeration(EnumerationImpl& el) { m_enumeration.set(el); }
            void setEnumeration(EnumerationPtr el) { m_enumeration.set(el); }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::EnumerationLiteral> {
        static std::string name() { return "EnumerationLiteral"; }
        template <class Policy>
        static SetList sets(UML::EnumerationLiteral<Policy>& el) {
            return SetList {
                make_set_pair("enumeration", el.getEnumerationSingleton())
            };
        }
    };
}
