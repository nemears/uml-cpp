#pragma once

#include <string>
#include "element.h"
#include "uml/set/set.h"
#include "uml/set/singleton.h"

namespace UML {

    class Namespace;
    class Dependency;
    typedef UmlPtr<Namespace> NamespacePtr;

    // namespace Parsers {
    //     void setNamespace(NamedElement& el, ID id);
    // }

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
        friend bool parseNamedElementScope(YAML::Node node, NamedElement& namedElement, ParserData& data);

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
            void referenceErased(ID id) override;
            ReadOnlySingleton<Namespace, NamedElement, UpdateQualifiedNamePolicy>& getNamespaceSingleton();
            NamedElement();
        public:
            virtual std::string getName();
            virtual void setName(const std::string &name);
            std::string getQualifiedName();
            NamespacePtr getNamespace() const;
            Set<Dependency, NamedElement, DoNothingPolicy>& getClientDependencies();
            VisibilityKind getVisibility();
            void setVisibility(VisibilityKind visibility);
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::NAMED_ELEMENT;
            };
        private:
            void setNamespace(ID id);
    };
}
