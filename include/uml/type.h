#ifndef TYPE
#define TYPE
#include "namedElement.h"
using namespace std;

namespace UML{
class Type : public NamedElement {
    public:
        virtual bool isPrimitive();
};
}

#endif