#ifndef PACKAGEABLE_ELEMENT_H
#define PACKAGEABLE_ELEMENT_H

#include "namedElement.h"
#include "parameterableElement.h"
#include "singleton.h"

namespace UML {

    class Package;
    namespace Parsers {
        class SetOwningPackage;
    }

    class PackageableElement : virtual public NamedElement , virtual public ParameterableElement {

        friend class Package;
        friend class Parsers::SetOwningPackage;

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
                    void operator()(ID id) const override;
            };
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
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