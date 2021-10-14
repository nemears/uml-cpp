#ifndef PACKAGE_MERGE_H
#define PACKAGE_MERGE_H

#include "directedRelationship.h"

namespace UML {
    
    class Package;

    class PackageMerge : public DirectedRelationship {

        friend class Package;
        friend class UmlManager;

        protected:
            Singleton<Package, PackageMerge> m_receivingPackage = Singleton<Package, PackageMerge>(this);
            class RemoveReceivingPackageProcedure : public AbstractSingletonProcedure<Package, PackageMerge> {
                public:
                    RemoveReceivingPackageProcedure(PackageMerge* me) : AbstractSingletonProcedure<Package, PackageMerge>(me) {};
                    void operator()(Package* el) const override;
            };
            class AddReceivingPackageProcedure : public AbstractSingletonProcedure<Package, PackageMerge> {
                public:
                    AddReceivingPackageProcedure(PackageMerge* me) : AbstractSingletonProcedure<Package, PackageMerge>(me) {};
                    void operator()(Package* el) const override;
            };
            Singleton<Package, PackageMerge> m_mergedPackage = Singleton<Package, PackageMerge>(this);
            class RemoveMergedPackageProcedure : public AbstractSingletonProcedure<Package, PackageMerge> {
                public:
                    RemoveMergedPackageProcedure(PackageMerge* me) : AbstractSingletonProcedure<Package, PackageMerge>(me) {};
                    void operator()(Package* el) const override;
            };
            class AddMergedPackageProcedure : public AbstractSingletonProcedure<Package, PackageMerge> {
                public:
                    AddMergedPackageProcedure(PackageMerge* me) : AbstractSingletonProcedure<Package, PackageMerge>(me) {};
                    void operator()(Package* el) const override;
            };
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            PackageMerge();
        public:
            PackageMerge(const PackageMerge& merge);
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
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PACKAGE_MERGE;
            };
    };
}

#endif