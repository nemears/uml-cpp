#ifndef VALUE_SPECIFICATION_H
#define VALUE_SPECIFICATION_H

#include "typedElement.h"
#include "packageableElement.h"

namespace UML {

    class ValueSpecification : public TypedElement , public PackageableElement {

        protected:
            void reindexName(std::string oldName, std::string newName) override;
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReference(Element* el) override;
            void referenceErased(ID id) override;
            ValueSpecification();
        public:
            ValueSpecification(const ValueSpecification& rhs);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::VALUE_SPECIFICATION;
            };
    };
}
#endif