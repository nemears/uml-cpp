#pragma once

#include "namedElement.h"
#include "directedRelationship.h"
#include "uml/set/singleton.h"
#include "uml/types/element.h"

namespace UML {

    class PackageableElement;
    class Namespace;
    typedef UmlPtr<PackageableElement> PackageableElementPtr;
    typedef UmlPtr<Namespace> NamespacePtr;

    class ElementImport : public DirectedRelationship {

        friend class Namespace;
        friend struct ElementInfo<ElementImport>;

        protected:
            struct ImportedElementPolicy {
                void elementAdded(PackageableElement& el, ElementImport& me);
                void elementRemoved(PackageableElement& el, ElementImport& me);
            };
            std::string m_alias = "";
            VisibilityKind m_visibility = VisibilityKind::PUBLIC;
            Singleton<PackageableElement, ElementImport, ImportedElementPolicy> m_importedElement = 
                Singleton<PackageableElement, ElementImport, ImportedElementPolicy>(this);
            Singleton<Namespace, ElementImport> m_importingNamespace = Singleton<Namespace, ElementImport>(this);
            Singleton<PackageableElement, ElementImport, ImportedElementPolicy>& getImportedElementSingleton();
            Singleton<Namespace, ElementImport>& getImportingNamespaceSingleton();
        public:
            ElementImport(std::size_t elementType, AbstractManager& manager);
            PackageableElementPtr getImportedElement() const;
            void setImportedElement(PackageableElement* importedElement);
            void setImportedElement(PackageableElement& importedElement);
            void setImportedElement(PackageableElementPtr importedElement);
            void setImportedElement(ID id);
            NamespacePtr getImportingNamespace() const;
            void setImportingNamespace(Namespace* importingNamespace);
            void setImportingNamespace(Namespace& importingNamespace);
            void setImportingNamespace(NamespacePtr importingNamespace);
            void setImportingNamespace(ID id);
            std::string getAlias() const;
            void setAlias(const std::string& alias);
            VisibilityKind getVisibility() const;
            void setVisibility(VisibilityKind visibility);
            typedef TypeInfo<std::tuple<DirectedRelationship>, ElementImport> Info;
    };

    template <>
    struct ElementInfo<ElementImport> : public DefaultInfo {
        static const bool abstract = false;
        static constexpr std::string_view name {"ElementImport"};
        static SetList sets(ElementImport& el) {
            return SetList{
                makeSetPair("importedElement", el.m_importedElement),
                makeSetPair("importingNamespace", el.m_importingNamespace)
            };
        }
    };
}
