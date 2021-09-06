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
            Singleton<ValueSpecification, Property> m_defaultValue = Singleton<ValueSpecification, Property>(this);
            class RemoveDefaultValueProcedure : public AbstractSingletonProcedure<ValueSpecification, Property> {
                public:
                    RemoveDefaultValueProcedure(Property* me) : AbstractSingletonProcedure<ValueSpecification, Property>(me) {};
                    void operator()(ID id, ValueSpecification* el) const override;
            };
            class AddDefaultValueProcedure : public AbstractSingletonProcedure<ValueSpecification, Property> {
                public:
                    AddDefaultValueProcedure(Property* me) : AbstractSingletonProcedure<ValueSpecification, Property>(me) {};
                    void operator()(ID id, ValueSpecification* el) const override;
            };
            Singleton<StructuredClassifier, Property> m_structuredClassifier = Singleton<StructuredClassifier, Property>(this);
            class RemoveStructuredClassifierProcedure : public AbstractSingletonProcedure<StructuredClassifier, Property> {
                public:
                    RemoveStructuredClassifierProcedure(Property* me) : AbstractSingletonProcedure<StructuredClassifier, Property>(me) {};
                    void operator()(ID id, StructuredClassifier* el) const override;
            };
            class AddStructuredClassifierProcedure : public AbstractSingletonProcedure<StructuredClassifier, Property> {
                public:
                    AddStructuredClassifierProcedure(Property* me) : AbstractSingletonProcedure<StructuredClassifier, Property>(me) {};
                    void operator()(ID id, StructuredClassifier* el) const override;
            };
            Singleton<Classifier, Property> m_classifier = Singleton<Classifier, Property>(this);
            class RemoveClassifierProcedure : public AbstractSingletonProcedure<Classifier, Property> {
                public:
                    RemoveClassifierProcedure(Property* me) : AbstractSingletonProcedure<Classifier, Property>(me) {};
                    void operator()(ID id, Classifier* el) const override;
            };
            class AddClassifierProcedure : public AbstractSingletonProcedure<Classifier, Property> {
                public:
                    AddClassifierProcedure(Property* me) : AbstractSingletonProcedure<Classifier, Property>(me) {};
                    void operator()(ID id, Classifier* el) const override;
            };
            Singleton<DataType, Property> m_dataType = Singleton<DataType, Property>(this);
            class RemoveDataTypeProcedure : public AbstractSingletonProcedure<DataType, Property> {
                public:
                    RemoveDataTypeProcedure(Property* me) : AbstractSingletonProcedure<DataType, Property>(me) {};
                    void operator()(ID id, DataType* el) const override;
                };
            class AddDataTypeProcedure : public AbstractSingletonProcedure<DataType, Property> {
                public:
                    AddDataTypeProcedure(Property* me) : AbstractSingletonProcedure<DataType, Property>(me) {};
                    void operator()(ID id, DataType* el) const override;
            };
            Singleton<Class, Property> m_class = Singleton<Class, Property>(this);
            class RemoveClassProcedure : public AbstractSingletonProcedure<Class, Property> {
                public:
                    RemoveClassProcedure(Property* me) : AbstractSingletonProcedure<Class, Property>(me) {};
                    void operator()(ID id, Class* el) const override;
            };
            class AddClassProcedure : public AbstractSingletonProcedure<Class, Property> {
                public:
                    AddClassProcedure(Property* me) : AbstractSingletonProcedure<Class, Property>(me) {};
                    void operator()(ID id, Class* el) const override;
            };
            Singleton<Association, Property> m_association = Singleton<Association, Property>(this);
            class RemoveAssociationProcedure : public AbstractSingletonProcedure<Association, Property> {
                public:
                    RemoveAssociationProcedure(Property* me) : AbstractSingletonProcedure<Association, Property>(me) {};
                    void operator()(ID id, Association* el) const override;
            };
            class AddAssociationProcedure : public AbstractSingletonProcedure<Association, Property> {
                public:
                    AddAssociationProcedure(Property* me) : AbstractSingletonProcedure<Association, Property>(me) {};
                    void operator()(ID id, Association* el) const override;
            };
            Singleton<Association, Property> m_owningAssociation = Singleton<Association, Property>(this);
            class RemoveOwningAssociationProcedure : public AbstractSingletonProcedure<Association, Property> {
                public:
                    RemoveOwningAssociationProcedure(Property* me) : AbstractSingletonProcedure<Association, Property>(me) {};
                    void operator()(ID id, Association* el) const override;
            };
            class AddOwningAssociationProcedure : public AbstractSingletonProcedure<Association, Property> {
                public:
                    AddOwningAssociationProcedure(Property* me) : AbstractSingletonProcedure<Association, Property>(me) {};
                    void operator()(ID id, Association* el) const override;
            };
            Singleton<Artifact, Property> m_artifact = Singleton<Artifact, Property>(this);
            class RemoveArtifactProcedure : public AbstractSingletonProcedure<Artifact, Property> {
                public:
                    RemoveArtifactProcedure(Property* me) : AbstractSingletonProcedure<Artifact, Property>(me) {};
                    void operator()(ID id, Artifact* el) const override;
            };
            class AddArtifactProcedure : public AbstractSingletonProcedure<Artifact, Property> {
                public:
                    AddArtifactProcedure(Property* me) : AbstractSingletonProcedure<Artifact, Property>(me) {};
                    void operator()(ID id, Artifact* el) const override;
            };
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