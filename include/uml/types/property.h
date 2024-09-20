#pragma once

#include "structuralFeature.h"
#include "connectableElement.h"
#include "deploymentTarget.h"
#include "uml/set/singleton.h"
#include "uml/types/element.h"
#include "uml/types/templateableElement.h"
#include "uml/umlPtr.h"

namespace UML{

    class Classifier;
    class DataType;
    class StructuredClassifier;
    class Association;
    class Class;
    class Interface;
    class ValueSpecification;
    class Type;

    typedef UmlPtr<Class> ClassPtr;
    typedef UmlPtr<DataType> DataTypePtr;
    typedef UmlPtr<Association> AssociationPtr;
    typedef UmlPtr<Interface> InterfacePtr;
    typedef UmlPtr<ValueSpecification> ValueSpecificationPtr;

    enum class AggregationKind {
        NONE,
        SHARED,
        COMPOSITE
    };

    class Property: public StructuralFeature, public ConnectableElement , public DeploymentTarget {

        friend class Class;
        friend class Classifier;
        friend class StructuredClassifier;
        friend class DataType;
        friend class Association;
        friend class Interface;
        friend struct ElementInfo<Property>;

        protected:
            AggregationKind m_aggregation = AggregationKind::NONE;
            bool m_composite = false;
            struct PropertyTypePolicy {
                void elementAdded(Type& el, Property& me);
                void elementRemoved(Type&el, Property& me);
            };
            struct RedefinedPropertyPolicy {
                void elementAdded(Property& el, Property& me);
                void elementRemoved(Property& el, Property& me);
            };
            Singleton<ValueSpecification, Property> m_defaultValue = Singleton<ValueSpecification, Property>(this);
            Singleton<DataType, Property> m_dataType = Singleton<DataType, Property>(this);
            Singleton<Class, Property> m_class = Singleton<Class, Property>(this);
            Singleton<Association, Property> m_association = Singleton<Association, Property>(this);
            Singleton<Association, Property> m_owningAssociation = Singleton<Association, Property>(this);
            Singleton<Interface, Property> m_interface = Singleton<Interface, Property>(this);
            Set<Property, Property, RedefinedPropertyPolicy> m_redefinedProperties = Set<Property, Property, RedefinedPropertyPolicy>(this);
            Singleton<Type, Property, PropertyTypePolicy> m_propertyType = Singleton<Type, Property, PropertyTypePolicy>(this);
            Set<Property, Property> m_subsettedProperties = Set<Property, Property>(this);
            Singleton<ValueSpecification, Property>& getDefaultValueSingleton();
            Singleton<Class, Property>& getClassSingleton();
            Singleton<DataType, Property>& getDataTypeSingleton();
            Singleton<Association, Property>& getAssociationSingleton();
            Singleton<Association, Property>& getOwningAssociationSingleton();
            Singleton<Interface, Property>& getInterfaceSingleton();
        public:
            Property(std::size_t elementType, AbstractManager& manager);
            AggregationKind getAggregation();
            bool isComposite();
            void setComposite(bool composite);
            void setAggregation(AggregationKind aggregation);
            void setDefaultValue(ValueSpecificationPtr val);
            void setDefaultValue(ValueSpecification& val);
            void setDefaultValue(ID id);
            ValueSpecificationPtr getDefaultValue() const;
            DataTypePtr getDataType() const;
            void setDataType(DataTypePtr dataType);
            void setDataType(DataType& dataType);
            void setDataType(ID id);
            ClassPtr getClass() const;
            void setClass(ClassPtr clazz);
            void setClass(Class& clazz);
            void setClass(ID id);
            AssociationPtr getAssociation() const;
            void setAssociation(AssociationPtr association);
            void setAssociation(Association& association);
            void setAssociation(ID id);
            AssociationPtr getOwningAssociation() const;
            void setOwningAssociation(AssociationPtr association);
            void setOwningAssociation(Association& association);
            void setOwningAssociation(ID id);
            InterfacePtr getInterface() const;
            void setInterface(InterfacePtr interface_uml);
            void setInterface(Interface& interface_uml);
            void setInterface(ID id);
            Set<Property, Property, RedefinedPropertyPolicy>& getRedefinedProperties();
            Set<Property, Property>& getSubsettedProperties();
            typedef TypeInfo<std::tuple<StructuralFeature, ConnectableElement, DeploymentTarget>, Property> Info;
    };

    template <>
    struct ElementInfo<Property> : public DefaultInfo {
        static const bool abstract = false;
        inline static std::string name{"Property"};
        static SetList sets(Property& el) {
            return SetList{
                makeSetPair("type", el.m_propertyType),
                makeSetPair("defaultValue", el.m_defaultValue),
                makeSetPair("dataType", el.m_dataType),
                makeSetPair("interface", el.m_interface),
                makeSetPair("owningAssociation", el.m_owningAssociation),
                makeSetPair("class", el.m_class),
                makeSetPair("association", el.m_association),
                makeSetPair("redefinedProperties", el.m_redefinedProperties),
                makeSetPair("subsettedProperties", el.m_subsettedProperties)
            };
        }
    };

    class ImproperRedefinitionException : public std::exception {
        public:
            virtual const char* what() const throw() {
                return "RedefinedProperty is not a member of a general classifier!";
            };
    };
}
