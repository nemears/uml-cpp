#ifndef _UML_PARAMETER_H_
#define _UML_PARAMETER_H_

#include "multiplicityElement.h"
#include "connectableElement.h"
#include "valueSpecification.h"

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
        DELETE
    };

    class Parameter : public ConnectableElement , public MultiplicityElement {

        friend class UmlManager;
        friend class Operation;

        protected:
            ParameterDirectionKind m_direction = ParameterDirectionKind::NONE;
            Singleton<Operation, Parameter> m_operation = Singleton<Operation, Parameter>(this);
            Singleton<ValueSpecification, Parameter> m_defaultValue = Singleton<ValueSpecification, Parameter>(this);
            bool m_isException = false;
            bool m_isStream = false;
            ParameterEffectKind m_effect = ParameterEffectKind::NONE;
            Set<Operation, Parameter>& getOperationSingleton();
            Set<ValueSpecification, Parameter>& getDefaultValueSingleton();
            void init();
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
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PARAMETER;
            };
    };
}
#endif