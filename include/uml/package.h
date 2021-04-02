#ifndef PACKAGE_H
#define PACKAGE_H

#include "packageableElement.h"
#include "namespace.h"

namespace UML {
    class Package : public PackageableElement, public Namespace {
        protected:
            Sequence<PackageableElement>* m_ownedPackages;
        public:
            Package();
            ~Package();
            Sequence<PackageableElement>& getOwnedPackages();
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif