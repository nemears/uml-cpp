#ifndef PROPERTYH
#define PROPERTYH
#include "structuralFeature.h"
#include "valueSpecification.h"
#include "multiplicityElement.h"
using namespace std;

namespace UML{
class Property: public StructuralFeature /*, public MultiplicityElement*/ {
    protected:
        ValueSpecification * defaultValue;
    public: 
        void setDefaultValue(ValueSpecification* val);
        ValueSpecification* getDefaultValue();
        ElementType getElementType() override;
        Property() {
            defaultValue = NULL;
        }
};
}
#endif