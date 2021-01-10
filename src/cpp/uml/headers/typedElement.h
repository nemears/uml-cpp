#ifndef TYPEDELEMENT
#define TYPEDELEMENT
#include "namedElement.h"
#include "type.h"
using namespace std;

namespace UML{
class TypedElement : public NamedElement {
    protected:
        Type* type;

    public:
        Type* getType();
        void setType(Type* type);
};
}

#endif