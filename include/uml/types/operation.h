#pragma once

#include "egm/egm-basic.h"
#include "uml/types/abstraction.h"

namespace UML {
    template <class>
    class BehavioralFeature;

    template <class>
    class DataType;

    template <class ManagerPolicy>
    class Operation : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<Operation, EGM::TemplateTypeList<BehavioralFeature>>;
        protected:
            using DataTypeSingleton = EGM::Singleton<DataType, Operation>;
            DataTypeSingleton m_dataType = DataTypeSingleton(this);
        private:
            void init() {
                m_dataType.subsets(ManagerPolicy::m_featuringClassifier);
                m_dataType.subsets(ManagerPolicy::m_namespace);
                m_dataType.opposite(&DataTypeSingleton::ManagedType::getOwnedOperations);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Operation);
            DataTypeSingleton& getDataTypeSingleton() { return m_dataType; }
            using DataTypeImpl = typename DataTypeSingleton::ManagedType;
            using DataTypePtr = EGM::ManagedPtr<DataTypeImpl>;
            DataTypePtr getDataType() { return m_dataType.get(); }
            void setDataType(DataTypeImpl& el) { m_dataType.set(el); }
            void setDataType(DataTypePtr el) { m_dataType.set(el); }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Operation> {
        static std::string name() { return "Operation"; }
        template <class Policy>
        static SetList sets(UML::Operation<Policy>& el) {
            return SetList {
                make_set_pair("dataType", el.getDataTypeSingleton())
            };
        }
    };
}
