#ifndef PROPERTY
#define PROPERTY
#include "typedElement.h"
#include "valueSpecification.h"
using namespace std;

namespace UML{
class Property: public TypedElement {
    public:
        ValueSpecification * defaultValue;
};
}
#endif