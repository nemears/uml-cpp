#ifndef TYPEDELEMENT
#define TYPEDELEMENT
#include "namedElement.h"
#include "type.h"
using namespace std;

class TypedElement : public NamedElement {
    protected:
        Type* type;

    public:
        Type* getType();
        void setType(Type* type);
};

#endif