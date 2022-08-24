#ifndef _UML_NAMED_ELEMENT_H_
#define _UML_NAMED_ELEMENT_H_

#include <string>
#include "element.h"
#include "singleton.h"

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
            Singleton<Namespace, NamedElement> m_namespace = Singleton<Namespace, NamedElement>(this);
            class UpdateQualifiedNameFunctor : public SetFunctor {
                public:
                    UpdateQualifiedNameFunctor(Element* them) : SetFunctor(them) {};
                    void operator()(Element& el) const override;
            };
            class RemoveQualifiedNameFunctor : public SetFunctor {
                public:
                    RemoveQualifiedNameFunctor(Element* them) : SetFunctor(them) {};
                    void operator()(Element& el) const override;
            };
            Set<Dependency, NamedElement>* m_clientDependencies;
            VisibilityKind m_visibility = VisibilityKind::PUBLIC;
            void reindexName(ID id, std::string newName) override;
            void updateQualifiedName(std::string absoluteNamespace);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            Set<Namespace, NamedElement>& getNamespaceSingleton();
            void init();
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