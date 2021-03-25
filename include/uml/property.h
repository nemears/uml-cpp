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
            Classifier* m_classifier;
            void reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) override;
            void reindexName(string oldName, string newName) override;
        public: 
            void setDefaultValue(ValueSpecification* val);
            ValueSpecification* getDefaultValue();
            Classifier* getClassifier();
            void setClassifier(Classifier* classifier);
            ElementType getElementType() override;
            Property() {
                defaultValue = NULL;
                m_classifier = NULL;
            }
    };
}
#endif