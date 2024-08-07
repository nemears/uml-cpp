#pragma once

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
            Type();
        public:
            virtual ~Type();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TYPE;
            };
    };
}
