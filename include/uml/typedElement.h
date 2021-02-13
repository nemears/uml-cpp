#ifndef TYPEDELEMENTH
#define TYPEDELEMENTH
#include "namedElement.h"
#include "type.h"
using namespace std;

namespace UML{
class TypedElement : virtual public NamedElement {
    protected:
        Type* type;

    public:
        Type* getType();
        void setType(Type* type);
        TypedElement() {
            type = 0;
        };
        ElementType getElementType() override;
};
}

#endif