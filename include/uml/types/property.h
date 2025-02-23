#pragma once 

#include "egm/egm-basic.h"
#include "egm/manager/typeInfo.h"
#include "uml/types/structuredClassifier.h"
#include "uml/util/indexableSet.h"

namespace UML {
    template <class>
    class StructuralFeature;

    template <class>
    class Class;

    template <class>
    class DataType;

    template <class>
    class ConnectableElement;

    template <class>
    class ValueSpecification;

    template <class>
    class Association;

    template <class>
    class Type;

    enum class AggregationKind {
        NONE,
        SHARED,
        COMPOSITE
    };

    template <class ManagerPolicy>
    class Property : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<Property, EGM::TemplateTypeList<StructuralFeature, ConnectableElement>>;
        protected:
            using ClassSingleton = EGM::Singleton<Class, Property>;
            using DataTypeSingleton = EGM::Singleton<DataType, Property>;
            using DefaultValue = EGM::Singleton<ValueSpecification, Property>;
            using RedefinedProperties = IndexableSet<Property, Property>;
            using SubsettedProperties = IndexableSet<Property, Property>;
            using AssociationSingleton = EGM::Singleton<Association, Property>;
            using OwningAssociation = EGM::Singleton<Association, Property>;
            struct PropertyTypePolicy {
                template <class TypeImpl>
                void elementAdded(TypeImpl& el, Property& me) {
                    if (me.getAssociation()) {
                        if (!me.getAssociation()->getEndTypes().contains(el)) {
                            me.addToReadonlySet(me.getAssociation()->getEndTypes(), el);
                        }
                    }
                }
                template <class TypeImpl>
                void elementRemoved(TypeImpl& el, Property& me) {
                    if (me.getAssociation()) {
                        if (me.getAssociation()->getEndTypes().contains(el)) {
                            me.removeFromReadonlySet(me.getAssociation()->getEndTypes(), el);
                        }
                    }
                }
            };
            using PropertyType = EGM::Singleton<Type, Property, PropertyTypePolicy>;
            ClassSingleton m_class = ClassSingleton(this);
            DataTypeSingleton m_dataType = DataTypeSingleton(this);
            AssociationSingleton m_association = AssociationSingleton(this);
            OwningAssociation m_owningAssociation = OwningAssociation(this);
            DefaultValue m_defaultValue = DefaultValue(this);
            RedefinedProperties m_redefinedProperties = RedefinedProperties(this);
            SubsettedProperties m_subsettedProperties = SubsettedProperties(this);
            PropertyType m_propertyType = PropertyType(this);
            AggregationKind m_aggregation = AggregationKind::NONE;
        private:
            void init() {
                m_class.subsets(ManagerPolicy::m_featuringClassifier);
                m_class.subsets(ManagerPolicy::m_namespace);
                m_class.opposite(&ClassSingleton::ManagedType::getOwnedAttributes);
                m_dataType.subsets(ManagerPolicy::m_featuringClassifier);
                m_dataType.subsets(ManagerPolicy::m_namespace);
                m_dataType.opposite(&DataTypeSingleton::ManagedType::getOwnedAttributes);
                m_association.opposite(&AssociationSingleton::ManagedType::getMemberEnds);
                m_owningAssociation.subsets(m_association);
                m_owningAssociation.subsets(ManagerPolicy::m_featuringClassifier);
                m_owningAssociation.subsets(ManagerPolicy::m_namespace);
                m_owningAssociation.opposite(&OwningAssociation::ManagedType::getOwnedEnds);
                m_defaultValue.subsets(ManagerPolicy::m_ownedElements);
                m_redefinedProperties.subsets(ManagerPolicy::m_redefinedElements);
                m_propertyType.redefines(ManagerPolicy::m_type);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Property);

            AggregationKind getAggregation() const { return m_aggregation; }
            void setAggregation(AggregationKind aggregtion) { 
                if (m_aggregation == aggregtion) {
                    return;
                }
                auto was_composite = isComposite();
                m_aggregation = aggregtion;
                auto is_composite = isComposite();
                auto featuring_classifier = ManagerPolicy::m_featuringClassifier.get();
                if (!featuring_classifier) {
                    return;
                }

                if (!featuring_classifier->template is<StructuredClassifier>()) {
                    return;
                }

                auto& structured_classifier = featuring_classifier->template as<StructuredClassifier>();
                
                if (!was_composite && is_composite) {
                    if (!structured_classifier.getParts().contains(*this)) {
                        this->addToReadonlySet(structured_classifier.getParts(), *this);
                    }
                } else if (was_composite && !is_composite) {
                    if (structured_classifier.getParts().contains(*this)) {
                        this->removeFromReadonlySet(structured_classifier.getParts(), *this);
                    } 
                }
            }
            bool isComposite() const { return m_aggregation == AggregationKind::COMPOSITE; }

            ClassSingleton& getClassSingleton() { return m_class; }
            using ClassImpl = typename ClassSingleton::ManagedType; 
            using ClassPtr = EGM::ManagedPtr<ClassImpl>;
            ClassPtr getClass() { return m_class.get(); }
            void setClass(ClassImpl& el) { m_class.set(el); }
            void setClass(ClassPtr el) { m_class.set(el); }

            DataTypeSingleton& getDataTypeSingleton() { return m_dataType; }
            using DataTypeImpl = typename DataTypeSingleton::ManagedType;
            using DataTypePtr = EGM::ManagedPtr<DataTypeImpl>;
            DataTypePtr getDataType() { return m_dataType.get(); }
            void setDataType(DataTypeImpl& el) { m_dataType.set(el); }
            void setDataType(DataTypePtr el) { m_dataType.set(el); }

            AssociationSingleton& getAssociationSingleton() { return m_association; }
            using AssociationImpl = typename AssociationSingleton::ManagedType;
            using AssociationPtr = EGM::ManagedPtr<AssociationImpl>;
            AssociationPtr getAssociation() const { return m_association.get(); }
            void setAssociation(AssociationImpl& el) { m_association.set(el); }
            void setAssociation(AssociationPtr el) { m_association.set(el); }

            OwningAssociation& getOwningAssociationSingleton() { return m_owningAssociation; }
            AssociationPtr getOwningAssociation() const { return m_owningAssociation.get(); }
            void setOwningAssociation(AssociationImpl& el) { m_owningAssociation.set(el); }
            void setOwningAssociation(AssociationPtr el) { m_owningAssociation.set(el); }

            DefaultValue& getDefaultValueSingleton() { return m_defaultValue; }
            using DefaultValueImpl = typename DefaultValue::ManagedType;
            using DefaultValuePtr = EGM::ManagedPtr<DefaultValueImpl>;
            DefaultValuePtr getDefaultValue() const { return m_defaultValue.get(); }
            void setDefaultValue(DefaultValueImpl& el) { m_defaultValue.set(el); }
            void setDefaultValue(DefaultValuePtr el) { m_defaultValue.set(el); }
            
            RedefinedProperties& getRedefinedProperties() { return m_redefinedProperties; }

            SubsettedProperties& getSubsettedProperties() { return m_subsettedProperties; }

            PropertyType& getTypeSingleton() { return m_propertyType; }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Property> {
        static std::string name() { return "Property"; }
        template <class Policy>
        static SetList sets(UML::Property<Policy>& el) {
            return SetList {
                make_set_pair("type", el.getTypeSingleton()),
                make_set_pair("class", el.getClassSingleton()),
                make_set_pair("dataType", el.getDataTypeSingleton()),
                make_set_pair("association", el.getAssociationSingleton()),
                make_set_pair("owningAssociation", el.getOwningAssociationSingleton()),
                make_set_pair("defaultValue", el.getDefaultValueSingleton()),
                make_set_pair("redefinedProperties", el.getRedefinedProperties()),
                make_set_pair("subsettedProperties", el.getSubsettedProperties())
            };
        } 
        template <class Policy>
        struct AggregationPolicy : public EGM::AbstractDataPolicy {
            ManagedPtr<UML::Property<Policy>> el;
            AggregationPolicy(UML::Property<Policy>& ref) : el(&ref) {}
            std::string getData() override {
                switch (el->getAggregation()) {
                    case UML::AggregationKind::NONE : return "";
                    case UML::AggregationKind::COMPOSITE : return "composite";
                    case UML::AggregationKind::SHARED : return "shared";
                }
            }
            void setData(std::string data) override {
                if (data == "composite") {
                    el->setAggregation(UML::AggregationKind::COMPOSITE);
                } else if (data == "shared") {
                    el->setAggregation(UML::AggregationKind::SHARED);
                }
            }
        };
        template <class Policy>
        static DataList data(UML::Property<Policy>& el) {
            return DataList {
                createDataPair<AggregationPolicy<Policy>>("aggregation", el)
            };
        }
    };
}
