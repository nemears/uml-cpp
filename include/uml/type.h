#ifndef TYPEH
#define TYPEH
#include "namedElement.h"
using namespace std;

namespace UML{
class Type : virtual public NamedElement {
    public:
        virtual bool isPrimitive() = 0;
        ElementType getElementType() override;
};
}

#endif