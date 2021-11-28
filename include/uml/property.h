#ifndef PROPERTYH
#define PROPERTYH

#include "structuralFeature.h"
#include "connectableElement.h"
// #include "deploymentTarget.h"

namespace UML{

    class Classifier;
    class DataType;
    class StructuredClassifier;
    class Association;
    class Class;
    class Artifact;
    namespace Parsers {
        class SetDefaultValue;
        class PropertySetClass;
        class PropertySetDataType;
        class PropertySetArtifact;
        class SetOwningAssociation;
        class SetAssociation;
    }

    enum class AggregationKind {
        NONE,
        SHARED,
        COMPOSITE
    };

    class Property: public StructuralFeature, public ConnectableElement/** , public DeploymentTarget**/ {

        friend class UmlManager;
        friend class Class;
        friend class Classifier;
        friend class StructuredClassifier;
        friend class DataType;

        protected:
            AggregationKind m_aggregation = AggregationKind::NONE;
            bool m_composite = false;
            Singleton<ValueSpecification, Property> m_defaultValue = Singleton<ValueSpecification, Property>(this);
            Singleton<DataType, Property> m_dataType = Singleton<DataType, Property>(this);
            Singleton<Class, Property> m_class = Singleton<Class, Property>(this);
            // Singleton<Association, Property> m_association = Singleton<Association, Property>(this);
            // Singleton<Association, Property> m_owningAssociation = Singleton<Association, Property>(this);
            // Singleton<Artifact, Property> m_artifact = Singleton<Artifact, Property>(this);
            // Sequence<Property> m_redefinedProperties = Sequence<Property>(this);
            // void reindexName(std::string oldName, std::string newName) override;
            void setComposite(bool composite);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void restoreReference(Element* el) override;
            void referenceErased(ID id) override;
            Set<ValueSpecification, Property>& getDefaultValueSingleton();
            Set<Class, Property>& getClassSingleton();
            Set<DataType, Property>& getDataTypeSingleton();
            void init();
            void copy(const Property& rhs);
            Property();
        public:
            Property(const Property& prop);
            virtual ~Property();
            AggregationKind getAggregation();
            bool isComposite();
            void setAggregation(AggregationKind aggregation);
            void setDefaultValue(ValueSpecification* val);
            void setDefaultValue(ValueSpecification& val);
            ValueSpecification* getDefaultValue();
            ValueSpecification& getDefaultValueRef();
            ID getDefaultValueID() const;
            bool hasDefaultValue() const;
            DataType* getDataType();
            DataType& getDataTypeRef();
            ID getDataTypeID() const;
            bool hasDataType() const;
            void setDataType(DataType* dataType);
            void setDataType(DataType& dataType);
            Class* getClass();
            Class& getClassRef();
            ID getClassID() const;
            bool hasClass() const;
            void setClass(Class* clazz);
            void setClass(Class& clazz);
            // Association* getAssociation();
            // Association& getAssociationRef();
            // ID getAssociationID() const;
            // bool hasAssociation() const;
            // void setAssociation(Association* association);
            // void setAssociation(Association& association);
            // Association* getOwningAssociation();
            // Association& getOwningAssociationRef();
            // ID getOwningAssociationID() const;
            // bool hasOwningAssociation() const;
            // void setOwningAssociation(Association* association);
            // void setOwningAssociation(Association& association);
            // virtual Artifact* getArtifact();
            // Artifact& getArtifactRef();
            // ID getArtifactID() const;
            // bool hasArtifact() const;
            // void setArtifact(Artifact* artifact);
            // void setArtifact(Artifact& artifact);
            // Sequence<Property>& getRedefinedProperties();
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