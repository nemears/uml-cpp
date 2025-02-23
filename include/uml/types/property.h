#pragma once 

#include "egm/egm-basic.h"
#include "uml/util/indexableSet.h"

namespace UML {
    template <class>
    class StructuralFeature;

    template <class>
    class Class;

    template <class>
    class DataType;

    template <class ManagerPolicy>
    class Property : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<Property, EGM::TemplateTypeList<StructuralFeature>>;
        protected:
            using ClassSingleton = EGM::Singleton<Class, Property>;
            using DataTypeSingleton = EGM::Singleton<DataType, Property>;
            using RedefinedProperties = IndexableSet<Property, Property>;
            ClassSingleton m_class = ClassSingleton(this);
            DataTypeSingleton m_dataType = DataTypeSingleton(this);
            RedefinedProperties m_redefinedProperties = RedefinedProperties(this);
        private:
            void init() {
                m_class.subsets(ManagerPolicy::m_featuringClassifier);
                m_class.subsets(ManagerPolicy::m_namespace);
                m_class.opposite(&ClassSingleton::ManagedType::getOwnedAttributes);
                m_dataType.subsets(ManagerPolicy::m_featuringClassifier);
                m_dataType.subsets(ManagerPolicy::m_namespace);
                m_dataType.opposite(&DataTypeSingleton::ManagedType::getOwnedAttributes);
                m_redefinedProperties.subsets(ManagerPolicy::m_redefinedElements);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Property);
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
            
            RedefinedProperties& getRedefinedProperties() { return m_redefinedProperties; }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Property> {
        static std::string name() { return "Property"; }
        template <class Policy>
        static SetList sets(UML::Property<Policy>& el) {
            return SetList {
                make_set_pair("class", el.getClassSingleton()),
                make_set_pair("dataType", el.getDataTypeSingleton()),
                make_set_pair("redefinedProperties", el.getRedefinedProperties())
            };
        } 
    };
}
