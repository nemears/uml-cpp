#pragma once

#include <string>
#include "element.h"

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
        friend struct ElementInfo<NamedElement>;

        protected:
            std::string m_name;
            std::string m_absoluteNamespace;
            class UpdateQualifiedNamePolicy {
                public:
                    void elementAdded(Namespace& el, NamedElement& me);
                    void elementRemoved(Namespace& el, NamedElement& me);
            };
            ReadOnlySingleton<Namespace, NamedElement, UpdateQualifiedNamePolicy> m_namespace = ReadOnlySingleton<Namespace, NamedElement, UpdateQualifiedNamePolicy>(this);
            Set<Dependency, NamedElement>  m_clientDependencies = Set<Dependency, NamedElement>(this);
            VisibilityKind m_visibility = VisibilityKind::PUBLIC;
            void updateQualifiedName(std::string absoluteNamespace);
            ReadOnlySingleton<Namespace, NamedElement, UpdateQualifiedNamePolicy>& getNamespaceSingleton();
            NamedElement(std::size_t elementType, AbstractManager& manager);
        public:
            virtual std::string getName();
            virtual void setName(const std::string &name);
            std::string getQualifiedName();
            NamespacePtr getNamespace() const;
            Set<Dependency, NamedElement, DoNothingPolicy>& getClientDependencies();
            VisibilityKind getVisibility();
            void setVisibility(VisibilityKind visibility);
            typedef TypeInfo<std::tuple<Element>, NamedElement> Info;
        private:
            void setNamespace(ID id);
    };

    template<>
    struct ElementInfo<NamedElement> : public DefaultInfo {
        inline static const std::string name {"NamedElement"};
        static SetList sets(NamedElement& el) {
            return SetList {
                std::make_pair<std::string, AbstractSet*>("namespace", &el.m_namespace),
                std::make_pair<std::string, AbstractSet*>("clientDependencies", &el.getClientDependencies())
            };
        }

        // Data
        static const bool extraData = true;
        struct NamedElementNameFunctor : public AbstractDataPolicy {
            NamedElement& el;
            NamedElementNameFunctor(NamedElement& el) : el(el) {}
            std::string getData() override {
                return el.getName();
            }
            void setData(std::string data) override {
                el.setName(data);
            }
        };
        struct NamedElementVisibilityFunctor : public AbstractDataPolicy {
            NamedElement& el;
            NamedElementVisibilityFunctor(NamedElement& el) : el(el) {}
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
        static DataList data(NamedElement& el) {
            return DataList {
                createDataPair("name", new NamedElementNameFunctor(el)),
                createDataPair("visibility", new NamedElementVisibilityFunctor(el))
            };
        }
    };

}
