#ifndef VALUESPECIFICATIONH
#define VALUESPECIFICATIONH
#include "typedElement.h"
#include "sequence.h"
#include "packageableElement.h"

namespace UML {
class ValueSpecification : public TypedElement , public PackageableElement {

    protected:
        Slot* m_owningSlot;
        void reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) override;
        void reindexName(std::string oldName, std::string newName) override;

    public:
        ValueSpecification();
        Slot* getOwningSlot();
        void setOwningSlot(Slot* slot);
        ElementType getElementType() const override;
        bool isSubClassOf(ElementType eType) override;
};
}
#endif