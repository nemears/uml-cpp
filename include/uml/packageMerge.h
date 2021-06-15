#ifndef PACKAGE_MERGE_H
#define PACKAGE_MERGE_H

#include "directedRelationship.h"

namespace UML {
    
    class Package;

    class PackageMerge : public DirectedRelationship {
        protected:
            ID m_receivingPackageID;
            Package* m_receivingPackagePtr;
            ID m_mergedPackageID;
            Package* m_mergedPackagePtr;
        public:
            PackageMerge();
            PackageMerge(const PackageMerge& merge);
            Package* getReceivingPackage();
            void setReceivingPackage(Package* receive);
            Package* getMergedPackage();
            void setMergedPackage(Package* merge);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
            static ElementType elementType() {
                return ElementType::PACKAGE_MERGE;
            };
    };
}

#endif