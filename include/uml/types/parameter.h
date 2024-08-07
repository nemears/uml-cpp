#pragma once

#include "multiplicityElement.h"
#include "connectableElement.h"

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

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class Operation;

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
            void referenceErased(ID id) override;
            Parameter();
        public:
            virtual ~Parameter();
            OperationPtr getOperation() const;
            void setOperation(Operation& operation);
            void setOperation(Operation* operation);
            void setOperation(ID id);
            ParameterDirectionKind getDirection();
            void setDirection(ParameterDirectionKind direction);
            ValueSpecificationPtr getDefaultValue() const;
            void setDefaultValue(ValueSpecification* defaultValue);
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
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PARAMETER;
            };
    };
}
