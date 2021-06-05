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
    class Class;

    enum class AggregationKind {
        NONE,
        SHARED,
        COMPOSITE
    };

    class Property: public StructuralFeature, public ConnectableElement {
        protected:
            AggregationKind m_aggregation;
            bool m_composite;
            ID m_defaultValueID;
            ValueSpecification* m_defaultValuePtr;
            StructuredClassifier* m_structuredClassifier;
            Classifier* m_classifier;
            DataType* m_dataType;
            Class* m_class;
            Association* m_association;
            Association* m_owningAssociation;
            void reindexID(ID oldID, ID newID) override;
            // void reindexName(std::string oldName, std::string newName) override;
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
            Class* getClass();
            void setClass(Class* clazz);
            Association* getAssociation();
            void setAssociation(Association* association);
            Association* getOwningAssociation();
            void setOwningAssociation(Association* association);
            void setType(Type* type) override;
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
            static ElementType elementType() {
                return ElementType::PROPERTY;
            };
    };
}
#endif