#ifndef PROPERTYH
#define PROPERTYH
#include "structuralFeature.h"
#include "valueSpecification.h"
using namespace std;

namespace UML{
class Property: public StructuralFeature {
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