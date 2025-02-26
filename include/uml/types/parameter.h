#pragma once

#include "egm/egm-basic.h"
#include "egm/managedPtr.h"

namespace UML {
    template <class>
    class ConnectableElement;

    template <class>
    class MultiplicityElement;

    template <class>
    class ValueSpecification;

    template <class>
    class Operation;

    enum class ParameterDirectionKind {
        IN,
        INOUT,
        OUT,
        RETURN
    };

    template <class ManagerPolicy>
    class Parameter : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<Parameter, EGM::TemplateTypeList<ConnectableElement, MultiplicityElement>>;
        protected:
            using DefaultValue = EGM::Singleton<ValueSpecification, Parameter>;
            using OperationSingleton = EGM::Singleton<Operation, Parameter>;
            DefaultValue m_defaultValue = DefaultValue(this);
            OperationSingleton m_operation = OperationSingleton(this);
            ParameterDirectionKind m_direction = ParameterDirectionKind::IN;
        private:
            void init() {
                m_defaultValue.subsets(ManagerPolicy::m_ownedElements);
                m_operation.subsets(ManagerPolicy::m_feature);
                m_operation.subsets(ManagerPolicy::m_ownedMembers);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Parameter);

            DefaultValue& getDefaultValueSingleton() { return m_defaultValue; }
            using ValueImpl = typename DefaultValue::ManagedType;
            using ValuePtr = EGM::ManagedPtr<ValueImpl>;
            ValuePtr getDefaultValue() const { return m_defaultValue.get(); }
            void setDefaultValue(ValueImpl& el) { m_defaultValue.set(el); }
            void setDefaultValue(ValuePtr el) { m_defaultValue.set(el); }

            OperationSingleton& getOperationSingleton() { return m_operation; }
            using OperationImpl = typename OperationSingleton::ManagedType;
            using OperationPtr = EGM::ManagedPtr<OperationImpl>;
            OperationPtr getOperation() const { return m_operation.get(); }
            void setOperation(OperationImpl& el) { m_operation.set(el); }
            void setOperation(OperationPtr el) { m_operation.set(el); }

            ParameterDirectionKind getDirection() const { return m_direction; }
            void setDirection(ParameterDirectionKind direction) { m_direction = direction; }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Parameter> {
        static std::string name() { return "Parameter"; }
        template <class Policy>
        static SetList sets(UML::Parameter<Policy>& el) {
            return SetList {
                make_set_pair("defaultValue", el.getDefaultValueSingleton()),
                make_set_pair("operation", el.getOperationSingleton())
            };
        }
        template <class Policy>
        struct DirectionPolicy : public EGM::AbstractDataPolicy {
            ManagedPtr<UML::Parameter<Policy>> el;
            DirectionPolicy(UML::Parameter<Policy>& ref) : el(&ref) {}
            std::string getData() override {
                switch (el->getDirection()) {
                    case UML::ParameterDirectionKind::IN : return "";
                    case UML::ParameterDirectionKind::INOUT : return "inout";
                    case UML::ParameterDirectionKind::OUT : return "out";
                    case UML::ParameterDirectionKind::RETURN : return "return";
                }
            }
            void setData(std::string data) {
                if (data == "inout") {
                    el->setDirection(UML::ParameterDirectionKind::INOUT);
                } else if (data == "out") {
                    el->setDirection(UML::ParameterDirectionKind::OUT);
                } else if (data == "return") {
                    el->setDirection(UML::ParameterDirectionKind::RETURN);
                }
            }
        };
        template <class Policy>
        static DataList data(UML::Parameter<Policy>& el) {
            return DataList {
                createDataPair<DirectionPolicy<Policy>>("direction", el)
            };
        }
    };
}
