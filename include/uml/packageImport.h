#ifndef _UML_PACKAGE_IMPORT_H_
#define _UML_PACKAGE_IMPORT_H_

#include "directedRelationship.h"
#include "singleton.h"
#include "uml/macros.h"

namespace UML {

    class Package;
    typedef UmlPtr<Package> PackagePtr;
    typedef UmlPtr<Namespace> NamespacePtr;

    class PackageImport : public DirectedRelationship {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;
        friend class Namespace;

        protected:
            DEFINE_SINGLETON(ImportedPackage, m_importedPackage, Package, PackageImport)
            DEFINE_SINGLETON(ImportingNamespace, m_importingNamespace, Namespace, PackageImport)
            class AddImportedPackageFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    AddImportedPackageFunctor(Element* el) : SetFunctor(el) {};
            };
            class RemoveImportedPackageFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    RemoveImportedPackageFunctor(Element* el) : SetFunctor(el) {};
            };
            class PackageAddPackageableElementFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    PackageImport* m_import = 0;
                    PackageAddPackageableElementFunctor(Element* el) : SetFunctor(el) {};
            };
            class PackageRemovePackageableElementFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    PackageImport* m_import = 0;
                    PackageRemovePackageableElementFunctor(Element* el) : SetFunctor(el) {};
            };
            void init();
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