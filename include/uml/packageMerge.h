#ifndef _UML_PACKAGE_MERGE_H_
#define _UML_PACKAGE_MERGE_H_

#include "directedRelationship.h"
#include "singleton.h"

namespace UML {
    
    class Package;

    typedef UmlPtr<Package> PackagePtr;

    class PackageMerge : public DirectedRelationship {

        friend class Package;
        friend class UmlManager;

        protected:
            Singleton<Package, PackageMerge> m_receivingPackage = Singleton<Package, PackageMerge>(this);
            Singleton<Package, PackageMerge> m_mergedPackage = Singleton<Package, PackageMerge>(this);
            class MergedPackageReferenceFunctor : public SetFunctor {
                public:
                    MergedPackageReferenceFunctor(Element* them) : SetFunctor(them) {};
                    void operator()(Element& el) const override;
            };
            class RemoveMergedPackageReferenceFunctor : public SetFunctor {
                public:
                    RemoveMergedPackageReferenceFunctor(Element* them) : SetFunctor(them) {};
                    void operator()(Element& el) const override;
            };
            Set<Package, PackageMerge>& getReceivingPackageSingleton();
            Set<Package, PackageMerge>& getMergedPackageSingleton();
            void init();
            PackageMerge();
        public:
            virtual ~PackageMerge();
            PackagePtr getReceivingPackage();
            void setReceivingPackage(Package& receive);
            void setReceivingPackage(Package* receive);
            void setReceivingPackage(ID id);
            PackagePtr getMergedPackage();
            void setMergedPackage(Package& merge);
            void setMergedPackage(Package* merge);
            void setMergedPackage(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PACKAGE_MERGE;
            };
    };
}

#endif