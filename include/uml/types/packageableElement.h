#pragma once

#include "namedElement.h"
#include "parameterableElement.h"

namespace UML {

    class Package;
    typedef UmlPtr<Package> PackagePtr;

    class PackageableElement : virtual public NamedElement , virtual public ParameterableElement {

        friend class Package;

        protected:
            CustomSingleton<Package, PackageableElement> m_owningPackage = CustomSingleton<Package, PackageableElement>(this);
            void referenceErased(ID id) override;
            TypedSet<Package, PackageableElement>& getOwningPackageSingleton();
            PackageableElement();
        public:
            virtual ~PackageableElement() {};
            PackagePtr getOwningPackage() const;
            void setOwningPackage(Package& package);
            void setOwningPackage(Package* package);
            void setOwningPackage(ID id);
            bool isSubClassOf(ElementType eType) const override;
            PackageableElement& operator=(PackageableElement&&) {
                return *this;
            };
            static ElementType elementType() {
                return ElementType::PACKAGEABLE_ELEMENT;
            };
    };
}