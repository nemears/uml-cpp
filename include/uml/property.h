#ifndef _UML_PROPERTY_H_
#define _UML_PROPERTY_H_

#include "structuralFeature.h"
#include "connectableElement.h"
#include "deploymentTarget.h"

namespace UML{

    class Classifier;
    class DataType;
    class StructuredClassifier;
    class Association;
    class Class;
    class Interface;

    typedef UmlPtr<Class> ClassPtr;
    typedef UmlPtr<DataType> DataTypePtr;
    typedef UmlPtr<Association> AssociationPtr;
    typedef UmlPtr<Interface> InterfacePtr;

    enum class AggregationKind {
        NONE,
        SHARED,
        COMPOSITE
    };

    class Property: public StructuralFeature, public ConnectableElement , public DeploymentTarget {

        template <typename AccessPolicy, typename SerializationPolicy> friend class Manager;
        friend class Class;
        friend class Classifier;
        friend class StructuredClassifier;
        friend class DataType;
        friend class Association;
        friend class Interface;

        protected:
            AggregationKind m_aggregation = AggregationKind::NONE;
            bool m_composite = false;
            class SetPropertyTypePolicy {
                public:
                    void apply(Type& el, Property& me);
            };
            class RemovePropertyTypePolicy {
                public:
                    void apply(Type& el, Property& me);
            };
            class AddRedefinedPropertyPolicy {
                public:
                    void apply(Property& el, Property& me);
            };
            class RemoveRedefinedPropertyPolicy {
                public:
                    void apply(Property& el, Property& me);
            };
            CustomSingleton<ValueSpecification, Property> m_defaultValue = CustomSingleton<ValueSpecification, Property>(this);
            CustomSingleton<DataType, Property> m_dataType = CustomSingleton<DataType, Property>(this);
            CustomSingleton<Class, Property> m_class = CustomSingleton<Class, Property>(this);
            CustomSingleton<Association, Property> m_association = CustomSingleton<Association, Property>(this);
            CustomSingleton<Association, Property> m_owningAssociation = CustomSingleton<Association, Property>(this);
            CustomSingleton<Interface, Property> m_interface = CustomSingleton<Interface, Property>(this);
            CustomSet<Property, Property, AddRedefinedPropertyPolicy, RemoveRedefinedPropertyPolicy> m_redefinedProperties = CustomSet<Property, Property, AddRedefinedPropertyPolicy, RemoveRedefinedPropertyPolicy>(this);
            CustomSingleton<Type, Property, SetPropertyTypePolicy, RemovePropertyTypePolicy> m_propertyType = CustomSingleton<Type, Property, SetPropertyTypePolicy, RemovePropertyTypePolicy>(this);
            void setComposite(bool composite);
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
            void restoreReference(Element* el) override;
            TypedSet<ValueSpecification, Property>& getDefaultValueSingleton();
            TypedSet<Class, Property>& getClassSingleton();
            TypedSet<DataType, Property>& getDataTypeSingleton();
            TypedSet<Association, Property>& getAssociationSingleton();
            TypedSet<Association, Property>& getOwningAssociationSingleton();
            TypedSet<Interface, Property>& getInterfaceSingleton();
            Property();
        public:
            virtual ~Property();
            AggregationKind getAggregation();
            bool isComposite();
            void setAggregation(AggregationKind aggregation);
            void setDefaultValue(ValueSpecification* val);
            void setDefaultValue(ValueSpecification& val);
            void setDefaultValue(ID id);
            ValueSpecificationPtr getDefaultValue() const;
            DataTypePtr getDataType() const;
            void setDataType(DataType* dataType);
            void setDataType(DataType& dataType);
            void setDataType(ID id);
            ClassPtr getClass() const;
            void setClass(Class* clazz);
            void setClass(Class& clazz);
            void setClass(ID id);
            AssociationPtr getAssociation() const;
            void setAssociation(Association* association);
            void setAssociation(Association& association);
            void setAssociation(ID id);
            AssociationPtr getOwningAssociation() const;
            void setOwningAssociation(Association* association);
            void setOwningAssociation(Association& association);
            void setOwningAssociation(ID id);
            InterfacePtr getInterface() const;
            void setInterface(Interface* interface_uml);
            void setInterface(Interface& interface_uml);
            void setInterface(ID id);
            Set<Property, Property>& getRedefinedProperties();
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