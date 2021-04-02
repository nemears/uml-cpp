#ifndef PACKAGEABLE_ELEMENT_H
#define PACKAGEABLE_ELEMENT_H

#include "namedElement.h"

namespace UML {

    class Package;

    class PackageableElement : virtual public NamedElement {
        protected:
            Package* m_owningPackage;
        public:
            PackageableElement();
            Package* getOwningPackage();
            void setOwningPackage(Package* package);
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType) override;
            PackageableElement& operator=(PackageableElement&&) {
                return *this;
            };
    };
}

#endif