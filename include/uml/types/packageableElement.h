#pragma once

#include "namedElement.h"
#include "parameterableElement.h"
#include "uml/set/singleton.h"

namespace UML {

    class Package;
    typedef UmlPtr<Package> PackagePtr;

    class PackageableElement : virtual public NamedElement , virtual public ParameterableElement {

        friend class Package;

        protected:
            Singleton<Package, PackageableElement> m_owningPackage = Singleton<Package, PackageableElement>(this);
            void referenceErased(ID id) override;
            Singleton<Package, PackageableElement>& getOwningPackageSingleton();
            PackageableElement();
        public:
            virtual ~PackageableElement() {};
            PackagePtr getOwningPackage() const;
            void setOwningPackage(Package& package);
            void setOwningPackage(Package* package);
            void setOwningPackage(ID id);
            bool is(ElementType eType) const override;
            PackageableElement& operator=(PackageableElement&&) {
                return *this;
            };
            static ElementType elementType() {
                return ElementType::PACKAGEABLE_ELEMENT;
            };
    };
}
