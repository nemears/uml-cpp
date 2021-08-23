#ifndef PROPERTYH
#define PROPERTYH

#include "structuralFeature.h"
#include "valueSpecification.h"
#include "connectableElement.h"
#include "deploymentTarget.h"

namespace UML{

    class Classifier;
    class DataType;
    class StructuredClassifier;
    class Association;
    class Class;
    class Artifact;

    enum class AggregationKind {
        NONE,
        SHARED,
        COMPOSITE
    };

    class Property: public StructuralFeature, public ConnectableElement , public DeploymentTarget {
        friend class UmlManager;
        protected:
            AggregationKind m_aggregation;
            bool m_composite;
            ID m_defaultValueID;
            ValueSpecification* m_defaultValuePtr;
            ID m_structuredClassifierID;
            StructuredClassifier* m_structuredClassifierPtr;
            ID m_classifierID;
            Classifier* m_classifierPtr;
            ID m_dataTypeID;
            DataType* m_dataTypePtr;
            ID m_classID;
            Class* m_classPtr;
            ID m_associationID;
            Association* m_associationPtr;
            ID m_owningAssociationID;
            Association* m_owningAssociationPtr;
            ID m_artifactID;
            Artifact* m_artifactPtr;
            Sequence<Property> m_redefinedProperties = Sequence<Property>(this);
            void reindexID(ID oldID, ID newID) override;
            void reindexName(std::string oldName, std::string newName) override;
            void setComposite(bool composite);
            class AddRedefinedPropertyFunctor : public TemplateAbstractSequenceFunctor<Property,Property> {
                public:
                    AddRedefinedPropertyFunctor(Property* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Property& el) const override;
            };
            class CheckRedefinedPropertyFunctor : public TemplateAbstractSequenceFunctor<Property,Property> {
                public:
                    CheckRedefinedPropertyFunctor(Property* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Property& el) const override;
            };
            class RemoveRedefinedPropertyFunctor : public TemplateAbstractSequenceFunctor<Property,Property> {
                public:
                    RemoveRedefinedPropertyFunctor(Property* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Property& el) const override;
            };
            void setManager(UmlManager* manager) override;
            void restoreReleased(ID id, Element* released) override;
            void referencingReleased(ID id) override;
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
            Artifact* getArtifact();
            void setArtifact(Artifact* artifact);
            void setType(Type* type) override;
            Sequence<Property>& getRedefinedProperties();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PROPERTY;
            };
    };

    class ImproperRedefinitionException : public std::exception {
        public:
            virtual const char* what() const throw() {
                return "RedefinedProperty is not a member of a general classifier!";
            };
    };
}
#endif