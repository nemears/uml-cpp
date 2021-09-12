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
            class RemoveOwningPackageProcedure : public AbstractSingletonProcedure<Package, PackageableElement> {
                public:
                    RemoveOwningPackageProcedure(PackageableElement* me) : AbstractSingletonProcedure<Package, PackageableElement>(me) {};
                    void operator()(Package* el) const override;
            };
            class AddOwningPackageProcedure : public AbstractSingletonProcedure<Package, PackageableElement> {
                public:
                    AddOwningPackageProcedure(PackageableElement* me) : AbstractSingletonProcedure<Package, PackageableElement>(me) {};
                    void operator()(Package* el) const override;
            };
            void restoreReleased(ID id, Element* released) override;
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            PackageableElement();
        public:
            PackageableElement(const PackageableElement& el);
            virtual ~PackageableElement() {};
            Package* getOwningPackage();
            Package& getOwningPackageRef();
            bool hasOwningPackage() const;
            void setOwningPackage(Package& package);
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