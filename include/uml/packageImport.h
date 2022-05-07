#ifndef _UML_PACKAGE_IMPORT_H_
#define _UML_PACKAGE_IMPORT_H_

#include "directedRelationship.h"
#include "singleton.h"

namespace UML {

    typedef UmlPtr<Package> PackagePtr;
    typedef UmlPtr<Namespace> NamespacePtr;

    class PackageImport : public DirectedRelationship {

        friend class UmlManager;
        friend class Namespace;

        protected:
            Singleton<Package, PackageImport> m_importedPackage = Singleton<Package, PackageImport>(this);
            Singleton<Namespace, PackageImport> m_importingNamespace = Singleton<Namespace, PackageImport>(this);
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
            Set<Package, PackageImport>& getImportedPackageSingleton();
            Set<Namespace, PackageImport>& getImportingNamespaceSingleton();
            void init();
            PackageImport();
        public:
            virtual ~PackageImport();
            PackagePtr getImportedPackage() const;
            void setImportedPackage(Package* pckg);
            void setImportedPackage(Package& pckg);
            void setImportedPackage(PackagePtr pckg);
            void setImportedPackage(ID id);
            NamespacePtr getImportingNamespace() const;
            void setImportingNamespace(Namespace* nmspc);
            void setImportingNamespace(Namespace& nmspc);
            void setImportingNamespace(NamespacePtr nmspc);
            void setImportingNamespace(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PACKAGE_IMPORT;
            }
    };
}

#endif