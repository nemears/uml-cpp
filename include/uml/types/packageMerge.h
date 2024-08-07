#pragma once

#include "directedRelationship.h"
#include "uml/set/singleton.h"

namespace UML {
    
    class Package;

    typedef UmlPtr<Package> PackagePtr;

    class PackageMerge : public DirectedRelationship {

        friend class Package;
        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            Singleton<Package, PackageMerge> m_receivingPackage = Singleton<Package, PackageMerge>(this);
            Singleton<Package, PackageMerge> m_mergedPackage = Singleton<Package, PackageMerge>(this);
            Singleton<Package, PackageMerge>& getReceivingPackageSingleton();
            Singleton<Package, PackageMerge>& getMergedPackageSingleton();
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
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PACKAGE_MERGE;
            };
    };
}
