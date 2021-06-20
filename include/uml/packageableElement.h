#ifndef PACKAGEABLE_ELEMENT_H
#define PACKAGEABLE_ELEMENT_H

#include "namedElement.h"
#include "parameterableElement.h"

namespace UML {

    class Package;

    class PackageableElement : virtual public NamedElement , virtual public ParameterableElement {
        protected:
            ID m_owningPackageID;
            Package* m_owningPackagePtr;
        public:
            PackageableElement();
            PackageableElement(const PackageableElement& el);
            Package* getOwningPackage();
            void setOwningPackage(Package* package);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            PackageableElement& operator=(PackageableElement&&) {
                return *this;
            };
            static ElementType elementType() {
                return ElementType::PACKAGEABLE_ELEMENT;
            };
    };
}

#endif