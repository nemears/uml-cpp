#ifndef TYPEH
#define TYPEH
#include "namedElement.h"
using namespace std;

namespace UML{
class Type : public NamedElement {
    public:
        virtual bool isPrimitive() = 0;
        ElementType getElementType() override;
};
}

#endif