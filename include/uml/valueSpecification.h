#ifndef VALUESPECIFICATIONH
#define VALUESPECIFICATIONH
#include "typedElement.h"
#include "sequence.h"
#include "packageableElement.h"

namespace UML {
class ValueSpecification : public TypedElement , public PackageableElement {

    protected:
        void reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) override;
        void reindexName(string oldName, string newName) override;

    /**
     * perhaps return type void* function gets overiden but right now it is commented out
     * because it is too much time to implement and im lazy and dont see reason for it
     */

    //virtual void* getValue() = 0;
    public:
        ElementType getElementType() override;
        bool isSubClassOf(ElementType eType) override;
};
}
#endif