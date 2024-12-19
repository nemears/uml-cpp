#pragma once

#include "uml/managers/templateTypeList.h"
#include "uml/managers/typeInfo.h"
#include "uml/set/set.h"

namespace UML {
    template <class>
    class NamedElement;

    template <class ManagerPolicy>
    class Namespace : public ManagerPolicy {
        public:
            using Info = TypeInfo<Namespace, TemplateTypeList<NamedElement>>;
        protected:
            using NamedElementSet = ReadOnlySet<NamedElement, Namespace>;
            NamedElementSet m_members = NamedElementSet(this);
            NamedElementSet m_ownedMembers = NamedElementSet(this);
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

    template <>
    struct ElementInfo<Namespace> : public DefaultInfo {};
}
