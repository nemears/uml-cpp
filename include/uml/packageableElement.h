#ifndef PACKAGEABLE_ELEMENT_H
#define PACKAGEABLE_ELEMENT_H

#include "namedElement.h"
#include "parameterableElement.h"
#include "singleton.h"

namespace UML {

    class Package;

    class PackageableElement : virtual public NamedElement , virtual public ParameterableElement {

        friend class Package;

        protected:
            Singleton<Package, PackageableElement> m_owningPackage = Singleton<Package, PackageableElement>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReference(Element* el) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Set<Package, PackageableElement>& getOwningPackageSingleton();
            void init();
            void copy(const PackageableElement& rhs);
            PackageableElement();
        public:
            PackageableElement(const PackageableElement& el);
            virtual ~PackageableElement() {};
            Package* getOwningPackage();
            Package& getOwningPackageRef();
            ID getOwningPackageID() const;
            bool hasOwningPackage() const;
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

#endif