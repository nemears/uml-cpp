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

    template <class>
    class Classifier;

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
            void setName(std::string name) {
                m_name = name;
            }
            VisibilityKind getVisibility() {
                return m_visibility;
            }
            void setVisibility(VisibilityKind visibility) {
                using ClassifierPtr = EGM::ManagedPtr<Classifier<typename ManagerPolicy::manager::template GenBaseHierarchy<Classifier>>>;
                if (m_visibility != visibility) {
                    if (visibility == VisibilityKind::PRIVATE) {
                        std::vector<ClassifierPtr> clazzs;
                        for (auto& referencePair : ManagerPolicy::m_node.lock()->m_references) {
                            auto& reference = referencePair.second;
                            if (reference.m_node.lock()->m_ptr) {
                                EGM::ManagedPtr<typename ManagerPolicy::manager::BaseElement> reference_base = EGM::AbstractElementPtr(reference.m_node.lock()->m_ptr.get());
                                if(reference_base->template is<Classifier>()) {
                                    auto& el = reference_base->template as<Classifier>();
                                    if (el.getInheritedMembers().contains(ManagerPolicy::m_id)) {
                                        clazzs.emplace_back(&el);
                                    } 
                                }
                            }
                        }

                        for (auto& clazz : clazzs) {
                            clazz->getInheritedMembers().innerRemove(this);
                        }
                    }
                    m_visibility = visibility;
                }
            }
            std::string getQualifiedName() {
                std::string ret = m_name;
                auto curr =  m_namespace.get();
                while (curr) {
                    ret = curr->getName() + "::" + ret;
                    curr = curr->getNamespace();
                }
                return ret;
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
        static DataList data(UML::NamedElement<Policy>& el) {
            return DataList {
                createDataPair<NamePolicy<Policy>>("name", el),
                createDataPair<VisibilityPolicy<Policy>>("visibility", el)
            };
        }
    };
}
