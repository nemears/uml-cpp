#pragma once

#include "egm/egm-basic.h"
#include "uml/util/indexableSet.h"

namespace UML {
    template <class>
    class BehavioralFeature;

    template <class>
    class DataType;

    template <class>
    class Class;

    template <class ManagerPolicy>
    class Operation : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<Operation, EGM::TemplateTypeList<BehavioralFeature>>;
        protected:
            using DataTypeSingleton = EGM::Singleton<DataType, Operation>;
            using ClassSingleton = EGM::Singleton<Class, Operation>;
            using OwnedParameters = IndexableOrderedSet<Parameter, Operation>;
            DataTypeSingleton m_dataType = DataTypeSingleton(this);
            ClassSingleton m_class = ClassSingleton(this);
            OwnedParameters m_operationOwnedParameters = OwnedParameters(this);
        private:
            void init() {
                m_dataType.subsets(ManagerPolicy::m_featuringClassifier);
                m_dataType.subsets(ManagerPolicy::m_namespace);
                m_dataType.opposite(&DataTypeSingleton::ManagedType::getOwnedOperations);
                m_class.subsets(ManagerPolicy::m_featuringClassifier);
                m_class.subsets(ManagerPolicy::m_namespace);
                m_class.opposite(&ClassSingleton::ManagedType::getOwnedOperations);
                m_operationOwnedParameters.redefines(ManagerPolicy::m_ownedParameters);
                m_operationOwnedParameters.opposite(&OwnedParameters::ManagedType::getOperationSingleton);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Operation);

            DataTypeSingleton& getDataTypeSingleton() { return m_dataType; }
            using DataTypeImpl = typename DataTypeSingleton::ManagedType;
            using DataTypePtr = EGM::ManagedPtr<DataTypeImpl>;
            DataTypePtr getDataType() const { return m_dataType.get(); }
            void setDataType(DataTypeImpl& el) { m_dataType.set(el); }
            void setDataType(DataTypePtr el) { m_dataType.set(el); }

            ClassSingleton& getClassSingleton() { return m_class; }
            using ClassImpl = typename ClassSingleton::ManagedType;
            using ClassPtr = EGM::ManagedPtr<ClassImpl>;
            ClassPtr getClass() const { return m_class.get(); }
            void setClass(ClassImpl& el) { m_class.set(el); }
            void setClass(ClassPtr el) { m_class.set(el); }

            OwnedParameters& getOwnedParameters() { return m_operationOwnedParameters; }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Operation> {
        static std::string name() { return "Operation"; }
        template <class Policy>
        static SetList sets(UML::Operation<Policy>& el) {
            return SetList {
                make_set_pair("dataType", el.getDataTypeSingleton()),
                make_set_pair("class", el.getClassSingleton()),
                make_set_pair("ownedParameters", el.getOwnedParameters())
            };
        }
    };
}
