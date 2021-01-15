#ifndef PROPERTY
#define PROPERTY
#include "typedElement.h"
#include "valueSpecification.h"
using namespace std;

namespace UML{
class Property: public TypedElement {
    protected:
        ValueSpecification * defaultValue;
    public: 
        void setDefaultValue(ValueSpecification* val);
        ValueSpecification* getDefaultValue();
};
}
#endif