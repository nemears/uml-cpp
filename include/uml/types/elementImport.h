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
            class AddImportedElementPolicy {
                public:
                    static void apply(PackageableElement& el, ElementImport& me);
            };
            class RemoveImportedElementPolicy {
                public:
                    static void apply(PackageableElement& el, ElementImport& me);
            };
            std::string m_alias = "";
            VisibilityKind m_visibility = VisibilityKind::PUBLIC;
            CustomSingleton<PackageableElement, ElementImport, AddImportedElementPolicy, RemoveImportedElementPolicy> m_importedElement = CustomSingleton<PackageableElement, ElementImport, AddImportedElementPolicy, RemoveImportedElementPolicy>(this);
            CustomSingleton<Namespace, ElementImport> m_importingNamespace = CustomSingleton<Namespace, ElementImport>(this);
            TypedSet<PackageableElement, ElementImport>& getImportedElementSingleton();
            TypedSet<Namespace, ElementImport>& getImportingNamespaceSingleton();
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
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ELEMENT_IMPORT;
            };
    };
}

#endif