#pragma once

#include "egm/egm-basic.h"

namespace UML {

    template <class>
    class Element;

    template <class>
    class Namespace;

    template <class ManagerPolicy>
    class NamedElement : public ManagerPolicy {
        friend struct EGM::ElementInfo<NamedElement>;
        public:
            using Info = EGM::TypeInfo<NamedElement, EGM::TemplateTypeList<Element>>;
            using NamespaceSingleton = EGM::ReadOnlySingleton<Namespace, NamedElement>;
        protected:
            NamespaceSingleton m_namespace = NamespaceSingleton(this);
            void init() {
                m_namespace.subsets(ManagerPolicy::m_owner);
                m_namespace.opposite(&NamespaceSingleton::ManagedType::getOwnedMembers);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(NamedElement);
            NamespaceSingleton& getNamespaceSingleton() {
                return m_namespace;
            }
            using NamespacePtr = EGM::ManagedPtr<typename NamespaceSingleton::ManagedType>;
            NamespacePtr getNamespace() {
                return m_namespace.get();
            }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::NamedElement> {
        static std::string name() { return "NamedElement"; }
        static const bool abstract = true;
        template <class Policy>
        static SetList sets(UML::NamedElement<Policy>& el) {
            return SetList {
                make_set_pair("namespace", el.m_namespace)
            };
        }
    };
}
