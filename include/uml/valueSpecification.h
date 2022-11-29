#ifndef _UML_VALUE_SPECIFICATION_H_
#define _UML_VALUE_SPECIFICATION_H_

#include "typedElement.h"
#include "packageableElement.h"

namespace UML {

    class ValueSpecification : public TypedElement , public PackageableElement {

        protected:
            void referenceReindexed(ID newID) override;
            void restoreReference(Element* el) override;
            void referenceErased(ID id) override;
            ValueSpecification();
        public:
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::VALUE_SPECIFICATION;
            };
    };
}
#endif