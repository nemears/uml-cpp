#ifndef _UML_ELEMENT_IMPORT_H_
#define _UML_ELEMENT_IMPORT_H_

#include "directedRelationship.h"
#include "singleton.h"

namespace UML {

    typedef UmlPtr<PackageableElement> PackageableElementPtr;
    typedef UmlPtr<Namespace> NamespacePtr;

    class ElementImport : public DirectedRelationship {

        friend class UmlManager;
        friend class Namespace;

        protected:
            Singleton<PackageableElement, ElementImport> m_importedElement = Singleton<PackageableElement, ElementImport>(this);
            Singleton<Namespace, ElementImport> m_importingNamespace = Singleton<Namespace, ElementImport>(this);
            Set<PackageableElement, ElementImport>& getImportedElementSingleton();
            Set<Namespace, ElementImport>& getImportingNamespaceSingleton();
            void init();
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
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ELEMENT_IMPORT;
            };
    };
}

#endif