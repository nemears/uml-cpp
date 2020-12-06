#ifndef PROPERTY
#define PROPERTY
#include "typedElement.h"
#include "valueSpecification.h"
using namespace std;

class Property: public TypedElement {
    public:
        ValueSpecification * defaultValue;
};
#endif