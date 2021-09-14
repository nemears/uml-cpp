#ifndef TYPEH
#define TYPEH
#include "packageableElement.h"

namespace UML{

    class TypedElement;
    class Association;

    class Type : virtual public PackageableElement {

        friend class TypedElement;
        friend class Association;
        
        protected:
            void reindexID(ID oldID, ID newID) override;
            Type() {};
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