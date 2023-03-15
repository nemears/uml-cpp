#ifndef _UML_NAMED_ELEMENT_H_
#define _UML_NAMED_ELEMENT_H_

#include <string>
#include "element.h"
// #include"uml/setsingleton.h"

namespace UML{

    class Namespace;
    class Dependency;
    typedef UmlPtr<Namespace> NamespacePtr;

    namespace Parsers {
        void setNamespace(NamedElement& el, ID id);
    }

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
        friend void Parsers::setNamespace(NamedElement& el, ID id);

        protected:
            std::string m_name;
            std::string m_absoluteNamespace;
            class UpdateQualifiedNamePolicy {
                public:
                    void apply(Namespace& el, NamedElement& me);
            };
            class RemoveQualifiedNamePolicy {
                public:
                    void apply(Namespace& el, NamedElement& me);
            };
            CustomSingleton<Namespace, NamedElement, UpdateQualifiedNamePolicy, RemoveQualifiedNamePolicy>* m_namespace;
        protected:
            CustomSet<Dependency, NamedElement, DoNothingAdd<Dependency,NamedElement>, DoNothingRemove<Dependency,NamedElement>>* m_clientDependencies;
            VisibilityKind m_visibility = VisibilityKind::PUBLIC;
            void updateQualifiedName(std::string absoluteNamespace);
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
            TypedSet<Namespace, NamedElement>& getNamespaceSingleton();
            NamedElement();
        public:
            virtual ~NamedElement();
            virtual std::string getName();
            virtual void setName(const std::string &name);
            std::string getQualifiedName();
            NamespacePtr getNamespace() const;
            Set<Dependency, NamedElement>& getClientDependencies();
            VisibilityKind getVisibility();
            void setVisibility(VisibilityKind visibility);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::NAMED_ELEMENT;
            };
    };
}

#endif