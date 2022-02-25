#ifndef _UML_PACKAGEABLE_ELEMENT_H_
#define _UML_PACKAGEABLE_ELEMENT_H_

#include "namedElement.h"
// #include "parameterableElement.h"
#include "singleton.h"

namespace UML {

    class Package;
    typedef UmlPtr<Package> PackagePtr;

    class PackageableElement : virtual public NamedElement /**, virtual public ParameterableElement**/ {

        friend class Package;

        protected:
            Singleton<Package, PackageableElement> m_owningPackage = Singleton<Package, PackageableElement>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(std::string oldName, std::string newName) override;
            void restoreReference(Element* el) override;
            void referenceErased(ID id) override;
            Set<Package, PackageableElement>& getOwningPackageSingleton();
            void init();
            void copy(const PackageableElement& rhs);
            PackageableElement();
        public:
            PackageableElement(const PackageableElement& el);
            virtual ~PackageableElement() {};
            PackagePtr getOwningPackage();
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