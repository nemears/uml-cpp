#pragma once

#include <string>
#include "element.h"
#include "uml/managers/baseElement.h"
#include "uml/set/set.h"
#include "uml/set/singleton.h"

namespace UML {

    typedef UmlPtr<Namespace> NamespacePtr;

    enum class VisibilityKind {
        PUBLIC,
        PROTECTED,
        PRIVATE,
        PACKAGE
    };

    /**
     * A NamedElement is an Element in a model that may have a name
     **/
    class NamedElement : virtual public Element {

        friend class Namespace;
        friend class SetInfo<NamedElement>;

        protected:
            typedef TypeInfo<std::tuple<Element>, NamedElement> Info;
            std::string m_name;
            std::string m_absoluteNamespace;
            class UpdateQualifiedNamePolicy {
                public:
                    void elementAdded(Namespace& el, NamedElement& me);
                    void elementRemoved(Namespace& el, NamedElement& me);
            };
            ReadOnlySingleton<Namespace, NamedElement, UpdateQualifiedNamePolicy> m_namespace = ReadOnlySingleton<Namespace, NamedElement, UpdateQualifiedNamePolicy>(this);
            // Set<Dependency, NamedElement>  m_clientDependencies = Set<Dependency, NamedElement>(this);
            VisibilityKind m_visibility = VisibilityKind::PUBLIC;
            void updateQualifiedName(std::string absoluteNamespace);
            ReadOnlySingleton<Namespace, NamedElement, UpdateQualifiedNamePolicy>& getNamespaceSingleton();
            NamedElement(std::size_t elementType, AbstractManager& manager);
        public:
            virtual std::string getName();
            virtual void setName(const std::string &name);
            std::string getQualifiedName();
            NamespacePtr getNamespace() const;
            // Set<Dependency, NamedElement, DoNothingPolicy>& getClientDependencies();
            VisibilityKind getVisibility();
            void setVisibility(VisibilityKind visibility);
        private:
            void setNamespace(ID id);
    };

    template<>
    class SetInfo<NamedElement> {
        static SetList sets(NamedElement& el) {
            return SetList {
                std::make_pair<std::string, AbstractSet*>("namespace", &el.m_namespace)// ,
                // std::make_pair<std::string, AbstractSet*>("clientDependencies", &el.getClientDependencies())
            };
        }
    };

}
