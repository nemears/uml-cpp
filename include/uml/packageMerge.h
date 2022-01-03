#ifndef PACKAGE_MERGE_H
#define PACKAGE_MERGE_H

#include "directedRelationship.h"
#include "singleton.h"
#include "yaml-cpp/yaml.h"

namespace UML {
    
    class Package;
    namespace Parsers {
        void parsePackageMerge(YAML::Node node, PackageMerge& merge, Parsers::ParserMetaData& data);
    }

    class PackageMerge : public DirectedRelationship {

        friend class Package;
        friend class UmlManager;
        friend void Parsers::parsePackageMerge(YAML::Node node, PackageMerge& merge, Parsers::ParserMetaData& data);

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