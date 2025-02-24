#pragma once

#include "egm/egm-basic.h"
#include "uml/util/indexableSet.h"

namespace UML {
    template <class>
    class DataType;

    template <class>
    class EnumerationLiteral;

    template <class ManagerPolicy>
    class Enumeration : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<Enumeration, EGM::TemplateTypeList<DataType>>;
        protected:
            using OwnedLiterals = IndexableOrderedSet<EnumerationLiteral, Enumeration>;
            OwnedLiterals m_ownedLiterals = OwnedLiterals(this);
        private:
            void init() {
                m_ownedLiterals.subsets(ManagerPolicy::m_ownedMembers);
                m_ownedLiterals.opposite(&OwnedLiterals::ManagedType::getEnumerationSingleton);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Enumeration);
            OwnedLiterals& getOwnedLiterals() { return m_ownedLiterals; }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Enumeration> {
        static std::string name() { return "Enumeration"; }
        template <class Policy>
        static SetList sets(UML::Enumeration<Policy>& el) {
            return SetList {
                make_set_pair("ownedLiterals", el.getOwnedLiterals())
            };
        }
    };
}
