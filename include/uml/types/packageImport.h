#pragma once

#include "directedRelationship.h"
#include "uml/set/singleton.h"
#include "uml/types/package.h"

namespace UML {

    class Package;
    typedef UmlPtr<Package> PackagePtr;
    typedef UmlPtr<Namespace> NamespacePtr;

    class PackageImport : public DirectedRelationship {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class Namespace;

        protected:
            struct ImportedPackagePolicy {
                void elementAdded(Package& el, PackageImport& me);
                void elementRemoved(Package& el, PackageImport& me);
            };
            VisibilityKind m_visibility = VisibilityKind::PUBLIC;
            Singleton<Package, PackageImport, ImportedPackagePolicy> m_importedPackage = Singleton<Package, PackageImport, ImportedPackagePolicy>(this);
            Singleton<Namespace, PackageImport> m_importingNamespace = Singleton<Namespace, PackageImport>(this);
            Singleton<Package, PackageImport, ImportedPackagePolicy>& getImportedPackageSingleton();
            Singleton<Namespace, PackageImport>& getImportingNamespaceSingleton();
            // DEFINE_SINGLETON_W_POLICIES(ImportedPackage, m_importedPackage, Package, PackageImport, AddImportedPackagePolicy, RemoveImportedPackagePolicy)
            // DEFINE_SINGLETON(ImportingNamespace, m_importingNamespace, Namespace, PackageImport)
            PackageImport();
        public:
            virtual ~PackageImport();
            VisibilityKind getVisibility() const;
            void setVisibility(VisibilityKind visibility);
            void setImportedPackage(ID id);
            void setImportedPackage(PackagePtr ptr);
            void setImportedPackage(Package& pckg);
            PackagePtr getImportedPackage() const;
            void setImportingNamespace(ID id);
            void setImportingNamespace(NamespacePtr ptr);
            void setImportingNamespace(Namespace& nmspc);
            NamespacePtr getImportingNamespace() const;
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PACKAGE_IMPORT;
            }
    };
}
