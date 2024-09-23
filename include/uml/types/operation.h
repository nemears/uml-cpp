#pragma once

#include "behavioralFeature.h"
#include "templateableElement.h"
#include "parameterableElement.h"
#include "uml/set/orderedSet.h"
#include "uml/types/element.h"

namespace UML {

    class Class;
    class DataType;
    class Type;
    class Interface;

    typedef UmlPtr<Type> TypePtr;
    typedef UmlPtr<Class> ClassPtr;
    typedef UmlPtr<DataType> DataTypePtr;
    typedef UmlPtr<Interface> InterfacePtr;

    class Operation : public BehavioralFeature , public TemplateableElement, public ParameterableElement {
        
        friend class Class;
        friend class Parameter;
        friend class DataType;
        friend class Interface;
        friend struct ElementInfo<Operation>;

        protected:
            Singleton<Type, Operation> m_type = Singleton<Type, Operation>(this);
            Singleton<Class, Operation> m_class = Singleton<Class, Operation>(this);
            Singleton<DataType, Operation> m_dataType = Singleton<DataType, Operation>(this);
            Singleton<Interface, Operation> m_interface = Singleton<Interface, Operation>(this);
            OrderedSet<Parameter, Operation> m_operationOwnedParameters = OrderedSet<Parameter, Operation>(this);
            Singleton<Type, Operation>& getTypeSingleton();
            Singleton<Class, Operation>& getClassSingleton();
            Singleton<DataType, Operation>& getDataTypeSingleton();
            Singleton<Interface, Operation>& getInterfaceSingleton();
        public:
            Operation(std::size_t elementType, AbstractManager& manager);
            TypePtr getType() const;
            void setType(Type& type);
            void setType(TypePtr type);
            void setType(ID id);
            ClassPtr getClass() const;
            void setClass(Class& clazz);
            void setClass(ClassPtr clazz);
            void setClass(ID id);
            DataTypePtr getDataType() const;
            void setDataType(DataType& dataType);
            void setDataType(DataTypePtr dataType);
            void setDataType(ID id);
            InterfacePtr getInterface() const;
            void setInterface(InterfacePtr interface_uml);
            void setInterface(Interface& interface_uml);
            void setInterface(ID id);
            OrderedSet<Parameter, Operation>& getOwnedParameters();
            typedef TypeInfo<std::tuple<BehavioralFeature, TemplateableElement, ParameterableElement>, Operation> Info;
    };

    template <>
    struct ElementInfo<Operation> : public DefaultInfo {
        static const bool abstract = false;
        static constexpr std::string_view name {"Operation"};
        static SetList sets(Operation& el) {
            return SetList {
                makeSetPair("type", el.m_type),
                makeSetPair("class", el.m_class),
                makeSetPair("dataType", el.m_dataType),
                makeSetPair("interface", el.m_interface),
                makeSetPair("ownedParameters", el.m_operationOwnedParameters)
            };
        }
    };
}
