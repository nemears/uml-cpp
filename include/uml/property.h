#ifndef PROPERTYH
#define PROPERTYH
#include "structuralFeature.h"
#include "valueSpecification.h"
#include "multiplicityElement.h"

using namespace std;

namespace UML{

    class Classifier;

    class Property: public StructuralFeature , public MultiplicityElement {
        protected:
            ValueSpecification * defaultValue;
            Classifier* classifier;
        public: 
            void setDefaultValue(ValueSpecification* val);
            ValueSpecification* getDefaultValue();
            Classifier* getClassifer();
            void setClassifier(Classifier* classifier);
            ElementType getElementType() override;
            Property() {
                defaultValue = NULL;
                classifier = NULL;
            }
    };
}
#endif