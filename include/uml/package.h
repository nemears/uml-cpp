#ifndef PACKAGE_H
#define PACKAGE_H

#include "packageableElement.h"
#include "namespace.h"

namespace UML {
    class Package : public PackageableElement, public Namespace {
        protected:
            Sequence<PackageableElement> m_packagedElements;
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
        public:
            Package();
            ~Package();
            Package(const Package& pckg);
            Sequence<PackageableElement>& getPackagedElements();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif