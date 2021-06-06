#ifndef VALUESPECIFICATIONH
#define VALUESPECIFICATIONH
#include "typedElement.h"
#include "sequence.h"
#include "packageableElement.h"

namespace UML {
    class ValueSpecification : public TypedElement , public PackageableElement {

        protected:
            ID m_owningSlotID;
            Slot* m_owningSlotPtr;
            void reindexID(ID oldID, ID newID) override;
            // void reindexName(std::string oldName, std::string newName) override;

        public:
            ValueSpecification();
            Slot* getOwningSlot();
            void setOwningSlot(Slot* slot);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
            static ElementType elementType() {
                return ElementType::VALUE_SPECIFICATION;
            };
    };
}
#endif