#ifndef PACKAGE_H
#define PACKAGE_H

#include "packageableElement.h"
#include "namespace.h"

namespace UML {
    class Package : public PackageableElement, public Namespace {
        protected:
            Sequence<PackageableElement>* m_packagedElements;
            class AddPackagedElementFunctor : public AbstractSequenceFunctor {
                public:
                    AddPackagedElementFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
        public:
            Package();
            ~Package();
            Sequence<PackageableElement>& getPackagedElements();
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif