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

    enum class AggregationKind {
        NONE,
        SHARED,
        COMPOSITE
    };

    class Property: public StructuralFeature, public ConnectableElement {
        protected:
            AggregationKind m_aggregation;
            bool m_composite;
            ValueSpecification* defaultValue;
            StructuredClassifier* m_structuredClassifier;
            Classifier* m_classifier;
            DataType* m_dataType;
            Association* m_association;
            Association* m_owningAssociation;
            void reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) override;
            void reindexName(std::string oldName, std::string newName) override;
            void setComposite(bool composite);
        public: 
            Property();
            Property(const Property& prop);
            AggregationKind getAggregation();
            bool isComposite();
            void setAggregation(AggregationKind aggregation);
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
            void setType(Type* type) override;
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
    };
}
#endif