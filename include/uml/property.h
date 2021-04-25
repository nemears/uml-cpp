#ifndef PROPERTYH
#define PROPERTYH

#include "structuralFeature.h"
#include "valueSpecification.h"
#include "multiplicityElement.h"

namespace UML{

    class Classifier;
    class DataType;

    class Property: public StructuralFeature {
        protected:
            ValueSpecification * defaultValue;
            Classifier* m_classifier;
            DataType* m_dataType;
            void reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) override;
            void reindexName(std::string oldName, std::string newName) override;
        public: 
            void setDefaultValue(ValueSpecification* val);
            ValueSpecification* getDefaultValue();
            Classifier* getClassifier();
            void setClassifier(Classifier* classifier);
            DataType* getDataType();
            void setDataType(DataType* dataType);
            ElementType getElementType() override;
            Property();
            Property(const Property& prop);
            bool isSubClassOf(ElementType eType) override;
    };
}
#endif