#ifndef _UML_PACKAGE_MERGE_H_
#define _UML_PACKAGE_MERGE_H_

#include "directedRelationship.h"
#include "set/singleton.h"

namespace UML {
    
    class Package;

    typedef UmlPtr<Package> PackagePtr;

    class PackageMerge : public DirectedRelationship {

        friend class Package;
        template <typename AccessPolicy, typename SerializationPolicy> friend class Manager;

        protected:
            CustomSingleton<Package, PackageMerge> m_receivingPackage = CustomSingleton<Package, PackageMerge>(this);
            CustomSingleton<Package, PackageMerge> m_mergedPackage = CustomSingleton<Package, PackageMerge>(this);
            TypedSet<Package, PackageMerge>& getReceivingPackageSingleton();
            TypedSet<Package, PackageMerge>& getMergedPackageSingleton();
            PackageMerge();
        public:
            virtual ~PackageMerge();
            PackagePtr getReceivingPackage() const;
            void setReceivingPackage(Package& receive);
            void setReceivingPackage(Package* receive);
            void setReceivingPackage(ID id);
            PackagePtr getMergedPackage() const;
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