#pragma once

#include "directedRelationship.h"
#include "uml/set/singleton.h"
#include "uml/types/element.h"

namespace UML {
    
    class Package;

    typedef UmlPtr<Package> PackagePtr;

    class PackageMerge : public DirectedRelationship {

        friend class Package;
        friend struct ElementInfo<PackageMerge>;

        protected:
            Singleton<Package, PackageMerge> m_receivingPackage = Singleton<Package, PackageMerge>(this);
            Singleton<Package, PackageMerge> m_mergedPackage = Singleton<Package, PackageMerge>(this);
            Singleton<Package, PackageMerge>& getReceivingPackageSingleton();
            Singleton<Package, PackageMerge>& getMergedPackageSingleton();
        public:
            PackageMerge(std::size_t elementType, AbstractManager& manager);
            PackagePtr getReceivingPackage() const;
            void setReceivingPackage(Package& receive);
            void setReceivingPackage(PackagePtr receive);
            void setReceivingPackage(ID id);
            PackagePtr getMergedPackage() const;
            void setMergedPackage(Package& merge);
            void setMergedPackage(PackagePtr merge);
            void setMergedPackage(ID id);
            typedef TypeInfo<std::tuple<DirectedRelationship>, PackageMerge> Info;
    };

    template <>
    struct ElementInfo<PackageMerge> : public DefaultInfo {
        static const bool abstract = false;
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "PackageMerge"; }
        static SetList sets(PackageMerge& el) {
            return SetList{
                makeSetPair("receivingPackage", el.m_receivingPackage),
                makeSetPair("mergedPackage", el.m_mergedPackage)
            };
        }
    };
}
