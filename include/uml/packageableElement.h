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
            PackageableElement(const PackageableElement& el);
            Package* getOwningPackage();
            void setOwningPackage(Package* package);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
            PackageableElement& operator=(PackageableElement&&) {
                return *this;
            };
            static ElementType elementType() {
                return ElementType::PACKAGEABLE_ELEMENT;
            };
    };
}

#endif