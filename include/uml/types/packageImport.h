#pragma once

#include "directedRelationship.h"
#include "uml/set/singleton.h"
#include "uml/types/package.h"

namespace UML {

    class Package;
    typedef UmlPtr<Package> PackagePtr;
    typedef UmlPtr<Namespace> NamespacePtr;

    class PackageImport : public DirectedRelationship {

        friend class Namespace;
        friend struct ElementInfo<PackageImport>;

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
        public:
            PackageImport(std::size_t elementType, AbstractManager& manager);
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
            typedef TypeInfo<std::tuple<DirectedRelationship>, PackageImport> Info;
    };

    template <>
    struct ElementInfo<PackageImport> : public DefaultInfo {
        static const bool abstract = false;
        static constexpr std::string_view name{"PackageImport"};
        static SetList sets(PackageImport& el) {
            return SetList{
                std::make_pair<std::string, AbstractSet*>("importedPackage", &el.m_importedPackage),
                std::make_pair<std::string, AbstractSet*>("importingNamespace", &el.m_importingNamespace)
            };
        }
    };
}
