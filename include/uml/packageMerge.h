#ifndef PACKAGE_MERGE_H
#define PACKAGE_MERGE_H

#include "directedRelationship.h"
#include "singleton.h"
#include "singleton2.h"

namespace UML {
    
    class Package;

    class PackageMerge : public DirectedRelationship {

        friend class Package;
        friend class UmlManager;

        protected:
            Singleton2<Package, PackageMerge> m_receivingPackage = Singleton2<Package, PackageMerge>(this);
            Singleton2<Package, PackageMerge> m_mergedPackage = Singleton2<Package, PackageMerge>(this);
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
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Set<Package, PackageMerge>& getReceivingPackageSingleton();
            Set<Package, PackageMerge>& getMergedPackageSingleton();
            void init();
            void copy(const PackageMerge& rhs);
            PackageMerge();
        public:
            PackageMerge(const PackageMerge& rhs);
            virtual ~PackageMerge();
            Package* getReceivingPackage();
            Package& getReceivingPackageRef();
            ID getReceivingPackageID() const;
            bool hasReceivingPackage() const;
            void setReceivingPackage(Package& receive);
            void setReceivingPackage(Package* receive);
            Package* getMergedPackage();
            Package& getMergedPackageRef();
            ID getMergedPackageID() const;
            bool hasMergedPackage() const;
            void setMergedPackage(Package& merge);
            void setMergedPackage(Package* merge);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PACKAGE_MERGE;
            };
    };
}

#endif