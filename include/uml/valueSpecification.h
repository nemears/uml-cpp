#ifndef VALUE_SPECIFICATION_H
#define VALUE_SPECIFICATION_H

#include "typedElement.h"
#include "sequence.h"
#include "packageableElement.h"

namespace UML {
    class ValueSpecification : public TypedElement , public PackageableElement {
        protected:
            ID m_owningSlotID;
            Slot* m_owningSlotPtr;
            void reindexID(ID oldID, ID newID) override;
            void reindexName(std::string oldName, std::string newName) override;
            void restoreReleased(ID id, Element* released) override;
            void referencingReleased(ID id) override;
        public:
            ValueSpecification();
            Slot* getOwningSlot();
            void setOwningSlot(Slot* slot);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::VALUE_SPECIFICATION;
            };
    };
}
#endif