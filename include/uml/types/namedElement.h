#pragma once

#include "egm/egm-basic.h"
#include <string>

namespace UML {

    template <class>
    class Element;

    template <class>
    class Namespace;

    template <class>
    class Dependency;

    enum class VisibilityKind {
        NONE,
        PUBLIC,
        PROTECTED,
        PRIVATE,
        PACKAGE
    };

    template <class ManagerPolicy>
    class NamedElement : public ManagerPolicy {
        friend struct EGM::ElementInfo<NamedElement>;
        public:
            using Info = EGM::TypeInfo<NamedElement, EGM::TemplateTypeList<Element>>;
            using NamespaceSingleton = EGM::ReadOnlySingleton<Namespace, NamedElement>;
            using DependencySet = EGM::Set<Dependency, NamedElement>;
        protected:
            NamespaceSingleton m_namespace = NamespaceSingleton(this);
            DependencySet m_clientDependencies = DependencySet(this);
            std::string m_name = "";
            VisibilityKind m_visibility = VisibilityKind::NONE;
            void init() {
                m_namespace.subsets(ManagerPolicy::m_owner);
                m_namespace.opposite(&NamespaceSingleton::ManagedType::getOwnedMembers);
                m_clientDependencies.opposite(&DependencySet::ManagedType::getClients);
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
            DependencySet& getClientDependencies() { return m_clientDependencies; }
            std::string getName() {
                return m_name;
            }
            void setName(std::string& name) {
                m_name = name;
            }
            VisibilityKind getVisibility() {
                return m_visibility;
            }
            void setVisibility(VisibilityKind visibility) {
                // TODO
                m_visibility = visibility;
            }
            std::string getQualifiedName() {
                throw EGM::ManagerStateException("TODO implement qualifiedName");
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
                make_set_pair("namespace", el.m_namespace),
                make_set_pair("clientDependencies", el.m_clientDependencies)
            };
        }
        template <class Policy>
        struct NamePolicy : public EGM::AbstractDataPolicy {
            ManagedPtr<UML::NamedElement<Policy>> el;
            NamePolicy(UML::NamedElement<Policy>& ref) : el(&ref) {}
            std::string getData() override { return el->getName(); }
            void setData(std::string name) override { el->setName(name); }
        };
        template <class Policy>
        struct VisibilityPolicy : public EGM::AbstractDataPolicy {
            ManagedPtr<UML::NamedElement<Policy>> el;
            VisibilityPolicy(UML::NamedElement<Policy>& ref) : el(&ref) {}
            std::string getData() override {
                switch (el->getVisibility()) {
                    case UML::VisibilityKind::NONE: return "";
                    case UML::VisibilityKind::PACKAGE : return "PACKAGE";
                    case UML::VisibilityKind::PRIVATE : return "PRIVATE";
                    case UML::VisibilityKind::PROTECTED : return "PROTECTED";
                    case UML::VisibilityKind::PUBLIC : return "PUBLIC";
                }
            };
            void setData(std::string data) override {
                if (data == "PACKAGE") {
                    el->setVisibility(UML::VisibilityKind::PACKAGE);
                } else if (data == "PRIVATE") {
                    el->setVisibility(UML::VisibilityKind::PRIVATE);
                } else if (data == "PROTECTED") {
                    el->setVisibility(UML::VisibilityKind::PROTECTED);
                } else if (data == "PUBLIC") {
                    el->setVisibility(UML::VisibilityKind::PUBLIC);
                } else if (data == "") {
                    el->setVisibility(UML::VisibilityKind::NONE);
                }
            }
        };
        template <class Policy>
        DataList data(UML::NamedElement<Policy>& el) {
            return DataList {
                createDataPair<NamePolicy<Policy>>("name", el),
                createDataPair<VisibilityPolicy<Policy>>("visibility", el)
            };
        }
    };
}
