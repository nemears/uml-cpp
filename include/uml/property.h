#ifndef PROPERTYH
#define PROPERTYH

#include "structuralFeature.h"
#include "valueSpecification.h"
#include "connectableElement.h"

namespace UML{

    class Classifier;
    class DataType;
    class StructuredClassifier;
    class Association;

    class Property: public StructuralFeature, public ConnectableElement {
        protected:
            ValueSpecification * defaultValue;
            StructuredClassifier* m_structuredClassifier;
            Classifier* m_classifier;
            DataType* m_dataType;
            Association* m_association;
            Association* m_owningAssociation;
            void reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) override;
            void reindexName(std::string oldName, std::string newName) override;
        public: 
            void setDefaultValue(ValueSpecification* val);
            ValueSpecification* getDefaultValue();
            Classifier* getClassifier();
            void setClassifier(Classifier* classifier);
            StructuredClassifier* getStructuredClassifier();
            void setStructuredClassifier(StructuredClassifier* classifier);
            DataType* getDataType();
            void setDataType(DataType* dataType);
            Association* getAssociation();
            void setAssociation(Association* association);
            Association* getOwningAssociation();
            void setOwningAssociation(Association* association);
            ElementType getElementType() const override;
            Property();
            Property(const Property& prop);
            bool isSubClassOf(ElementType eType) override;
    };
}
#endif