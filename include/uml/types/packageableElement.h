#pragma once

#include "namedElement.h"
#include "parameterableElement.h"
#include "uml/managers/typeInfo.h"
#include "uml/set/singleton.h"

namespace UML {

    class Package;
    typedef UmlPtr<Package> PackagePtr;

    class PackageableElement : virtual public NamedElement , virtual public ParameterableElement {

        friend class Package;

        friend struct ElementInfo<PackageableElement>;

        protected:
            Singleton<Package, PackageableElement> m_owningPackage = Singleton<Package, PackageableElement>(this);
            Singleton<Package, PackageableElement>& getOwningPackageSingleton();
            PackageableElement(std::size_t elementType, AbstractManager& manager);
        public:
            PackagePtr getOwningPackage() const;
            void setOwningPackage(Package& package);
            void setOwningPackage(PackagePtr package);
            void setOwningPackage(ID id);
            typedef TypeInfo<std::tuple<NamedElement, ParameterableElement>, PackageableElement> Info;
    };

    template <>
    struct ElementInfo<PackageableElement> : public DefaultInfo {
        inline static const std::string name {"PackageableElement"};
        static SetList sets(PackageableElement& el) {
            return SetList {
                std::make_pair<std::string, AbstractSet*>("owningPackage", &el.m_owningPackage)
            };
        }
    };
}
