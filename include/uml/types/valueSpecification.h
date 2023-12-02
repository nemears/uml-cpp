#pragma once

#include "typedElement.h"
#include "packageableElement.h"

namespace UML {

    class ValueSpecification : public TypedElement , public PackageableElement {

        protected:
            void referenceErased(ID id) override;
            ValueSpecification();
        public:
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::VALUE_SPECIFICATION;
            };
    };
}