#pragma once

#include "egm/egm-basic.h"

namespace UML {
    template <class>
    class NamedElement;

    template <class ManagerPolicy>
    class Namespace : public ManagerPolicy {
        friend struct EGM::ElementInfo<Namespace>;
        public:
            using Info = EGM::TypeInfo<Namespace, EGM::TemplateTypeList<NamedElement>>;
        protected:
            using NamedElementSet = EGM::ReadOnlySet<NamedElement, Namespace>;
            NamedElementSet m_members = NamedElementSet(this);
            NamedElementSet m_ownedMembers = NamedElementSet(this);
        private:
            void init() {
                m_ownedMembers.subsets(ManagerPolicy::m_ownedElements);
                m_ownedMembers.subsets(m_members);
                m_ownedMembers.opposite(&NamedElementSet::ManagedType::getNamespaceSingleton);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Namespace);
            NamedElementSet& getMembers() {
                return m_members;
            }
            NamedElementSet& getOwnedMembers() {
                return m_ownedMembers;
            }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Namespace> {
        static std::string name() { return "Namespace"; }
        static const bool abstract = true;
        template <class Policy>
        static SetList sets(UML::Namespace<Policy>& el) {
            return SetList {
                make_set_pair("members", el.m_members),
                make_set_pair("ownedMembers", el.m_ownedMembers)
            };
        }
    };
}
