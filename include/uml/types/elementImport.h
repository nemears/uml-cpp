#ifndef _UML_ELEMENT_IMPORT_H_
#define _UML_ELEMENT_IMPORT_H_

#include "directedRelationship.h"
#include "uml/set/singleton.h"

namespace UML {

    class PackageableElement;
    typedef UmlPtr<PackageableElement> PackageableElementPtr;
    typedef UmlPtr<Namespace> NamespacePtr;

    class ElementImport : public DirectedRelationship {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class Namespace;

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
            ElementImport();
        public:
            virtual ~ElementImport();
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
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ELEMENT_IMPORT;
            };
    };
}

#endif
