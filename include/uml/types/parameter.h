#pragma once

#include "multiplicityElement.h"
#include "connectableElement.h"
#include "valueSpecification.h"
#include "parameterSet.h"

namespace UML {

    class Operation;
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
            CustomSingleton<Operation, Parameter> m_operation = CustomSingleton<Operation, Parameter>(this);
            CustomSingleton<ValueSpecification, Parameter> m_defaultValue = CustomSingleton<ValueSpecification, Parameter>(this);
            CustomSet<ParameterSet, Parameter> m_parameterSets = CustomSet<ParameterSet, Parameter>(this);
            bool m_isException = false;
            bool m_isStream = false;
            ParameterEffectKind m_effect = ParameterEffectKind::NONE;
            TypedSet<Operation, Parameter>& getOperationSingleton();
            TypedSet<ValueSpecification, Parameter>& getDefaultValueSingleton();
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
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PARAMETER;
            };
    };
}