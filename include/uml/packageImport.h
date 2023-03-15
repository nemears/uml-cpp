#ifndef _UML_PACKAGE_IMPORT_H_
#define _UML_PACKAGE_IMPORT_H_

#include "directedRelationship.h"
#include "set/singleton.h"
#include "uml/macros.h"

namespace UML {

    class Package;
    typedef UmlPtr<Package> PackagePtr;
    typedef UmlPtr<Namespace> NamespacePtr;

    class PackageImport : public DirectedRelationship {

        template <typename AccessPolicy, typename SerializationPolicy> friend class Manager;
        friend class Namespace;

        protected:
            class AddImportedPackagePolicy {
                public:
                    static void apply(Package& el, PackageImport& me);
            };
            class RemoveImportedPackagePolicy {
                public:
                    static void apply(Package& el, PackageImport& me);
            };
            DEFINE_SINGLETON_W_POLICIES(ImportedPackage, m_importedPackage, Package, PackageImport, AddImportedPackagePolicy, RemoveImportedPackagePolicy)
            DEFINE_SINGLETON(ImportingNamespace, m_importingNamespace, Namespace, PackageImport)
            PackageImport();
        public:
            virtual ~PackageImport();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PACKAGE_IMPORT;
            }
    };
}

#endif