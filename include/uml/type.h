#ifndef TYPEH
#define TYPEH
#include "packageableElement.h"

namespace UML{

    class TypedElement;
    class Association;
    class Operation;

    class Type : virtual public PackageableElement {

        friend class TypedElement;
        friend class Association;
        friend class Operation;
        
        protected:
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