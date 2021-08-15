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
            Sequence<PackageableElement> m_packagedElements;
            Sequence<PackageMerge> m_packageMerge;
            Sequence<ProfileApplication> m_profileApplications;
            Sequence<Stereotype> m_ownedStereotypes;
            class AddPackagedElementFunctor : public AbstractSequenceFunctor {
                public:
                    AddPackagedElementFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemovePackagedElementFunctor : public AbstractSequenceFunctor {
                public:
                    RemovePackagedElementFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class AddPackageMergeFunctor : public AbstractSequenceFunctor {
                public:
                    AddPackageMergeFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemovePackageMergeFunctor : public AbstractSequenceFunctor {
                public:
                    RemovePackageMergeFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class AddProfileApplicationFunctor : public AbstractSequenceFunctor {
                public:
                    AddProfileApplicationFunctor(Element* me) : AbstractSequenceFunctor(me){};
                    void operator()(Element& el) const override;
            };
            class RemoveProfileApplicationFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveProfileApplicationFunctor(Element* me) : AbstractSequenceFunctor(me){};
                    void operator()(Element& el) const override;
            };
            class AddOwnedStereotypeFunctor : public AbstractSequenceFunctor {
                public:
                    AddOwnedStereotypeFunctor(Element* me) : AbstractSequenceFunctor(me){};
                    void operator()(Element& el) const override;
            };
            class RemoveOwnedStereotypeFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveOwnedStereotypeFunctor(Element* me) : AbstractSequenceFunctor(me){};
                    void operator()(Element& el) const override;
            };
            void setManager(UmlManager* manager) override;
            void restoreReleased(ID id, Element* released) override;
            void referencingReleased(ID id) override;
        public:
            Package();
            virtual ~Package();
            Package(const Package& pckg);
            Sequence<PackageableElement>& getPackagedElements();
            Sequence<PackageMerge>& getPackageMerge();
            Sequence<ProfileApplication>& getProfileApplications();
            Sequence<Stereotype>& getOwnedStereotypes();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PACKAGE;
            };
    };
}

#endif