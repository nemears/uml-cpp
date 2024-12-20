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
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "Parameter"; }
        static SetList sets(Parameter& el) {
            return SetList{
                makeSetPair("operation", el.m_operation),
                makeSetPair("defaultValue", el.m_defaultValue),
                makeSetPair("parameterSets", el.m_parameterSets)
            };
        }
        static const bool extraData = true;
        struct ParameterDirectionPolicy : public AbstractDataPolicy {
            Parameter& el;
            ParameterDirectionPolicy(Parameter& el) : el(el) {}
            std::string getData() override {
                switch (el.getDirection()) {
                    case ParameterDirectionKind::IN_UML : return "in";
                    case ParameterDirectionKind::INOUT : return "inout";
                    case ParameterDirectionKind::OUT_UML : return "out";
                    case ParameterDirectionKind::RETURN : return "return";
                    case ParameterDirectionKind::NONE : return "";
                }
                return "";
            }
            void setData(std::string data) override {
                if (data == "in") {
                    el.setDirection(ParameterDirectionKind::IN_UML);
                } else if (data == "inout") {
                    el.setDirection(ParameterDirectionKind::INOUT);
                } else if (data == "out") {
                    el.setDirection(ParameterDirectionKind::OUT_UML);
                } else if (data == "return") {
                    el.setDirection(ParameterDirectionKind::RETURN);
                }
            }
        };
        static DataList data(Parameter& el) {
            return DataList {
                createDataPair("direction", new ParameterDirectionPolicy(el))
            };
        }
    };
}
