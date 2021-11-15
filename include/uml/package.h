#ifndef PACKAGE_H
#define PACKAGE_H

#include "packageableElement.h"
#include "namespace.h"
#include "templateableElement.h"

namespace UML {

    class Stereotype;
    class ProfileApplication;
    class PackageMerge;

    class Package : public PackageableElement, public Namespace, public TemplateableElement {
        friend class UmlManager;
        protected:
            Set<PackageableElement, Package> m_packagedElements = Set<PackageableElement, Package>(this);
            Set<PackageMerge, Package> m_packageMerge = Set<PackageMerge, Package>(this);
            // Set<ProfileApplication, Package> m_profileApplications = Set<ProfileApplication, Package>(this);
            // Set<Stereotype, Package> m_ownedStereotypes = Set<Stereotype, Package>(this);
            // class AddPackagedElementFunctor : public TemplateAbstractSequenceFunctor<PackageableElement,Package> {
            //     public:
            //         AddPackagedElementFunctor(Package* me) : TemplateAbstractSequenceFunctor(me) {};
            //         void operator()(PackageableElement& el) const override;
            //         void operator()(ID id) const override;
            // };
            // class RemovePackagedElementFunctor : public TemplateAbstractSequenceFunctor<PackageableElement,Package> {
            //     public:
            //         RemovePackagedElementFunctor(Package* me) : TemplateAbstractSequenceFunctor(me) {};
            //         void operator()(PackageableElement& el) const override;
            // };
            // class AddPackageMergeFunctor : public TemplateAbstractSequenceFunctor<PackageMerge,Package> {
            //     public:
            //         AddPackageMergeFunctor(Package* me) : TemplateAbstractSequenceFunctor(me) {};
            //         void operator()(PackageMerge& el) const override;
            //         void operator()(ID id) const override;
            // };
            // class RemovePackageMergeFunctor : public TemplateAbstractSequenceFunctor<PackageMerge,Package> {
            //     public:
            //         RemovePackageMergeFunctor(Package* me) : TemplateAbstractSequenceFunctor(me) {};
            //         void operator()(PackageMerge& el) const override;
            // };
            // class AddProfileApplicationFunctor : public TemplateAbstractSequenceFunctor<ProfileApplication,Package> {
            //     public:
            //         AddProfileApplicationFunctor(Package* me) : TemplateAbstractSequenceFunctor(me){};
            //         void operator()(ProfileApplication& el) const override;
            //         void operator()(ID id) const override;
            // };
            // class RemoveProfileApplicationFunctor : public TemplateAbstractSequenceFunctor<ProfileApplication,Package> {
            //     public:
            //         RemoveProfileApplicationFunctor(Package* me) : TemplateAbstractSequenceFunctor(me){};
            //         void operator()(ProfileApplication& el) const override;
            // };
            // class AddOwnedStereotypeFunctor : public TemplateAbstractSequenceFunctor<Stereotype,Package> {
            //     public:
            //         AddOwnedStereotypeFunctor(Package* me) : TemplateAbstractSequenceFunctor(me){};
            //         void operator()(Stereotype& el) const override;
            //         void operator()(ID id) const override;
            // };
            // class RemoveOwnedStereotypeFunctor : public TemplateAbstractSequenceFunctor<Stereotype,Package> {
            //     public:
            //         RemoveOwnedStereotypeFunctor(Package* me) : TemplateAbstractSequenceFunctor(me){};
            //         void operator()(Stereotype& el) const override;
            // };
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            void init();
            void copy(const Package& rhs);
            Package();
        public:
            virtual ~Package();
            Package(const Package& rhs);
            Set<PackageableElement, Package>& getPackagedElements();
            Set<PackageMerge, Package>& getPackageMerge();
            // Set<ProfileApplication, Package>& getProfileApplications();
            // Set<Stereotype, Package>& getOwnedStereotypes();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PACKAGE;
            };
    };
}

#endif