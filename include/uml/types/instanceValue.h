#pragma once

#include "egm/egm-basic.h"
#include "egm/manager/typeInfo.h"

namespace UML {
    template <class>
    class ValueSpecification;

    template <class>
    class InstanceSpecification;

    template <class ManagerPolicy>
    class InstanceValue : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<InstanceValue, EGM::TemplateTypeList<ValueSpecification>>;
        protected:
            using Instance = EGM::Singleton<InstanceSpecification, InstanceValue>;
            Instance m_instance = Instance(this);
        private:
            void init() {}
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(InstanceValue);
            Instance& getInstanceSingleton() { return m_instance; }
            using InstanceImpl = typename Instance::ManagedType;
            using InstancePtr = EGM::ManagedPtr<InstanceImpl>;
            InstancePtr getInstance() const { return m_instance.get(); }
            void setInstance(InstanceImpl& el) { m_instance.set(el); }
            void setInstance(InstancePtr el) { m_instance.set(el); }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::InstanceValue> {
        static std::string name() { return "InstanceValue"; }
        template <class Policy>
        static SetList sets(UML::InstanceValue<Policy>& el) {
            return SetList {
                make_set_pair("instance", el.getInstanceSingleton())
            };
        }
    };
}
