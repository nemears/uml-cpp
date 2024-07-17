#pragma once

#include <memory>
#include <string>
#include "element.h"
#include "uml/set/doNothingPolicy.h"

namespace UML {

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
        friend bool parseNamedElementScope(YAML::Node node, NamedElement& namedElement, ParserData& data);

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
            class QualifiedNamePolicy {
                // todo
            };
            std::unique_ptr<Singleton<Namespace, NamedElement, QualifiedNamePolicy>> m_namespace;
            std::unique_ptr<Set<Dependency, NamedElement, DoNothingPolicy>>  m_clientDependencies;
            VisibilityKind m_visibility = VisibilityKind::PUBLIC;
            void updateQualifiedName(std::string absoluteNamespace);
            void referenceErased(ID id) override;
            Set<Namespace, NamedElement, DoNothingPolicy>& getNamespaceSingleton();
            NamedElement();
        public:
            virtual ~NamedElement();
            virtual std::string getName();
            virtual void setName(const std::string &name);
            std::string getQualifiedName();
            NamespacePtr getNamespace() const;
            Set<Dependency, NamedElement, DoNothingPolicy>& getClientDependencies();
            VisibilityKind getVisibility();
            void setVisibility(VisibilityKind visibility);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::NAMED_ELEMENT;
            };
        private:
            void setNamespace(ID id);
    };
}
