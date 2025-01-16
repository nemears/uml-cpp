#pragma once

#include "uml/managers/dummyManager.h"
#include "uml/managers/templateTypeList.h"
#include "uml/managers/typeInfo.h"
#include "uml/set/singleton.h"
namespace UML {

    template <class>
    class Element;

    template <class>
    class Namespace;

    template <class ManagerPolicy>
    class NamedElement : public ManagerPolicy {
        friend struct ElementInfo<NamedElement>;
        public:
            using Info = TypeInfo<NamedElement, TemplateTypeList<Element>>;
            using NamespaceSingleton = ReadOnlySingleton<Namespace, NamedElement>;
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
            using NamespacePtr = UmlPtr<typename NamespaceSingleton::ManagedType>;
            NamespacePtr getNamespace() {
                return m_namespace.get();
            }
    };

    template <>
    struct ElementInfo<NamedElement> {
        template <class Policy>
        static SetList sets(NamedElement<Policy>& el) {
            return SetList {
                make_set_pair("namespace", el.m_namespace)
            };
        }
    };
}
