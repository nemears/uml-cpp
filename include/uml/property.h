#ifndef PROPERTY
#define PROPERTY
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
        class InvalidValueException: public exception {
            virtual const char* what() const throw() {
                return "tried to assign property value that does not match it's corresponded type";
            }
        } invalidValueException;
};
}
#endif