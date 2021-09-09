#ifndef TYPEH
#define TYPEH
#include "packageableElement.h"

namespace UML{

    class TypedElement;

    class Type : virtual public PackageableElement {

        friend class TypedElement;
        
        protected:
            void reindexID(ID oldID, ID newID) override;
        public:
            ElementType getElementType() const override;
            Type& operator=(Type&&) {
                return *this;
            };
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TYPE;
            };
    };
}

#endif