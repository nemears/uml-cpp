#pragma once

#include "multiplicityElement.h"
#include "connectableElement.h"
#include "uml/managers/abstractManager.h"
#include "uml/types/element.h"

namespace UML {

    class Operation;
    class ParameterSet;
    typedef UmlPtr<Operation> OperationPtr;
    namespace Parsers {
        class SetOperation;
        class SetBehavior;
    }

    enum class ParameterDirectionKind {
        IN_UML, 
        INOUT, 
        OUT_UML, 
        RETURN,
        NONE
    };

    enum class ParameterEffectKind {
        NONE,
        CREATE,
        READ,
        UPDATE,
        DELETE_UML
    };

    class Parameter : public ConnectableElement , public MultiplicityElement {

        friend class Operation;
        friend struct ElementInfo<Parameter>;

        protected:
            ParameterDirectionKind m_direction = ParameterDirectionKind::NONE;
            Singleton<Operation, Parameter> m_operation = Singleton<Operation, Parameter>(this);
            Singleton<ValueSpecification, Parameter> m_defaultValue = Singleton<ValueSpecification, Parameter>(this);
            Set<ParameterSet, Parameter> m_parameterSets = Set<ParameterSet, Parameter>(this);
            bool m_isException = false;
            bool m_isStream = false;
            ParameterEffectKind m_effect = ParameterEffectKind::NONE;
            Singleton<Operation, Parameter>& getOperationSingleton();
            Singleton<ValueSpecification, Parameter>& getDefaultValueSingleton();
        public:
            Parameter(std::size_t elementType, AbstractManager& manager);
            OperationPtr getOperation() const;
            void setOperation(Operation& operation);
            void setOperation(OperationPtr operation);
            void setOperation(ID id);
            ParameterDirectionKind getDirection();
            void setDirection(ParameterDirectionKind direction);
            ValueSpecificationPtr getDefaultValue() const;
            void setDefaultValue(ValueSpecification& defaultValue);
            void setDefaultValue(ValueSpecificationPtr defaultValue);
            void setDefaultValue(ID id);
            ParameterEffectKind getEffect() const;
            void setEffect(ParameterEffectKind kind);
            bool isException() const;
            void setIsException(bool isException);
            bool isStream() const;
            void setIsStream(bool isStream);
            Set<ParameterSet, Parameter>& getParameterSets();
            typedef TypeInfo<std::tuple<ConnectableElement, MultiplicityElement>, Parameter> Info;
    };

    template <>
    struct ElementInfo<Parameter> : public DefaultInfo {
        static const bool abstract = false;
        inline static const std::string name {"Parameter"};
        static SetList sets(Parameter& el) {
            return SetList{
                makeSetPair("operation", el.m_operation),
                makeSetPair("defaultValue", el.m_defaultValue),
                makeSetPair("parameterSets", el.m_parameterSets)
            };
        }
    };
}
