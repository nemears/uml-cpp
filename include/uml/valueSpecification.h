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

    /**
     * perhaps return type void* function gets overiden but right now it is commented out
     * because it is too much time to implement and im lazy and dont see reason for it
     */

    //virtual void* getValue() = 0;
    public:
        ValueSpecification();
        Slot* getOwningSlot();
        void setOwningSlot(Slot* slot);
        ElementType getElementType() const override;
        bool isSubClassOf(ElementType eType) override;
};
}
#endif