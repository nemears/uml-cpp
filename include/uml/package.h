#ifndef PACKAGE_H
#define PACKAGE_H

#include "packageableElement.h"
#include "namespace.h"
#include "packageMerge.h"
#include "templateableElement.h"

namespace UML {
    class Package : public PackageableElement, public Namespace, public TemplateableElement {
        friend class UmlManager;
        protected:
            Sequence<PackageableElement> m_packagedElements;
            Sequence<PackageMerge> m_packageMerge;
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
            void setManager(UmlManager* manager) override;
        public:
            Package();
            virtual ~Package();
            Package(const Package& pckg);
            Sequence<PackageableElement>& getPackagedElements();
            Sequence<PackageMerge>& getPackageMerge();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
            static ElementType elementType() {
                return ElementType::PACKAGE;
            };
    };
}

#endif