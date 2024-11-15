#pragma once

#include <string>
#include "element.h"

namespace UML {

    enum class VisibilityKind {
        PUBLIC,
        PROTECTED,
        PRIVATE,
        PACKAGE
    };

    /**
     * A NamedElement is an Element in a model that may have a name
     **/
    template <class ManagerPolicy>
    class NamedElement : public ManagerPolicy {

        template <class>
        friend class Namespace;
        friend struct ElementInfo<NamedElement>;

        using ManagedNamespace = Namespace<typename ManagerPolicy::Manager::template GenBaseHierarchy<Namespace>>;
        using NamespacePtr = UmlPtr<ManagedNamespace>;

        protected:
            std::string m_name;
            std::string m_absoluteNamespace;
            void updateQualifiedName(std::string absoluteNamespace) {
                m_absoluteNamespace = absoluteNamespace;
            }
            class UpdateQualifiedNamePolicy {
                    using ManagedType = Namespace<typename ManagerPolicy::Manager::template GenBaseHierarchy<Namespace>>;
                public:
                    void elementAdded(ManagedType& el, NamedElement& me) {
                        me.updateQualifiedName(el.getQualifiedName());
                    }
                    void elementRemoved(ManagedType& el, NamedElement& me);
            };
            ReadOnlySingleton<Namespace, NamedElement, UpdateQualifiedNamePolicy> m_namespace = ReadOnlySingleton<Namespace, NamedElement, UpdateQualifiedNamePolicy>(this);
            // Set<Dependency, NamedElement>  m_clientDependencies = Set<Dependency, NamedElement>(this);
            VisibilityKind m_visibility = VisibilityKind::PUBLIC;
            ReadOnlySingleton<Namespace, NamedElement, UpdateQualifiedNamePolicy>& getNamespaceSingleton() {
                return m_namespace;
            }
            NamedElement(std::size_t elementType, AbstractManager& manager) : ManagerPolicy(elementType, manager) {
                m_namespace.subsets(ManagerPolicy::m_owner);
                m_namespace.opposite(&ManagedNamespace::getOwnedMembers);
                // m_clientDependencies.opposite(&Dependency<typename ManagerPolicy::Manager::template GenBaseHierarchy<Dependency>>::getClients);
            } 
        public:
            virtual std::string getName() {
                return m_name;
            }
            virtual void setName(const std::string &name) {
                m_name = name;
            }
            std::string getQualifiedName() {
                return (getNamespace().has() ? m_absoluteNamespace + "::" : "") +  m_name;
            }
            NamespacePtr getNamespace() const {
                return m_namespace.get();
            }
            // Set<Dependency, NamedElement, DoNothingPolicy>& getClientDependencies() {
            //     return m_clientDependencies;
            // }
            VisibilityKind getVisibility() {
                return m_visibility;
            }
            void setVisibility(VisibilityKind visibility) {
                if (m_visibility != visibility) {
                    if (visibility == VisibilityKind::PRIVATE) {
                        // std::vector<ClassifierPtr> clazzs;
                        // for (auto& referencePair : ManagerPolicy::m_node.lock()->m_references) {
                        //     auto& reference = referencePair.second;
                        //     if (
                        //         reference.m_node.lock()->m_ptr &&
                        //         std::dynamic_pointer_cast<BaseElement<UmlTypes>>(reference.m_node.lock()->m_ptr)->is<Classifier>()
                        //     ) {
                        //         Classifier& el = std::dynamic_pointer_cast<BaseElement<UmlTypes>>(reference.m_node.lock()->m_ptr)->as<Classifier>();
                        //         if (el.getInheritedMembers().contains(ManagerPolicy::m_id)) {
                        //             clazzs.emplace_back(&el);
                        //         }
                        //     }
                        // }

                        // for (auto& clazz : clazzs) {
                        //     clazz->m_inheritedMembers.innerRemove(this);
                        // }
                    }
                }
                m_visibility = visibility; 
            }
            typedef TypeInfo<TemplateTypeList<Element>, NamedElement> Info;
        private:
            void setNamespace(ID id);
    };

    template<class ManagerPolicy>
    struct ElementInfo<NamedElement<ManagerPolicy>> : public DefaultInfo {
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "NamedElement"; }
        static SetList sets(NamedElement<ManagerPolicy>& el) {
            return SetList {
                std::make_pair<std::string, AbstractSet*>("namespace", &el.m_namespace)
                // std::make_pair<std::string, AbstractSet*>("clientDependencies", &el.getClientDependencies())
            };
        }

        // Data
        static const bool extraData = true;
        struct NamedElementNameFunctor : public AbstractDataPolicy {
            NamedElement<ManagerPolicy>& el;
            NamedElementNameFunctor(NamedElement<ManagerPolicy>& el) : el(el) {}
            std::string getData() override {
                return el.getName();
            }
            void setData(std::string data) override {
                el.setName(data);
            }
        };
        struct NamedElementVisibilityFunctor : public AbstractDataPolicy {
            NamedElement<ManagerPolicy>& el;
            NamedElementVisibilityFunctor(NamedElement<ManagerPolicy>& el) : el(el) {}
            std::string getData() override {
                switch (el.getVisibility()) {
                    case VisibilityKind::PUBLIC : return ""; // public is default so just return empty string
                    case VisibilityKind::PACKAGE : return "package";
                    case VisibilityKind::PROTECTED : return "protected";
                    case VisibilityKind::PRIVATE : return "private";
                }
                throw ManagerStateException("bad state, invalid visbility kind");
            }
            void setData(std::string data) override {
                if (data == "public") {
                    el.setVisibility(VisibilityKind::PUBLIC);
                } else if (data == "protected") {
                    el.setVisibility(VisibilityKind::PROTECTED);
                } else if (data == "private") {
                    el.setVisibility(VisibilityKind::PRIVATE);
                } else if (data == "package") {
                    el.setVisibility(VisibilityKind::PACKAGE);
                } else {
                    // TODO change exception type maybe?
                    throw ManagerStateException("Bad visibility given to named element data!");
                }
            }
        };
        static DataList data(NamedElement<ManagerPolicy>& el) {
            return DataList {
                createDataPair("name", new NamedElementNameFunctor(el)),
                createDataPair("visibility", new NamedElementVisibilityFunctor(el))
            };
        }
    };
}
