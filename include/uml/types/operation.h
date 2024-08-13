#pragma once

#include "behavioralFeature.h"
#include "templateableElement.h"
#include "parameterableElement.h"
#include "uml/set/orderedSet.h"

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
        
        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class Class;
        friend class Parameter;
        friend class DataType;
        friend class Interface;

        protected:
            Singleton<Type, Operation> m_type = Singleton<Type, Operation>(this);
            Singleton<Class, Operation> m_class = Singleton<Class, Operation>(this);
            Singleton<DataType, Operation> m_dataType = Singleton<DataType, Operation>(this);
            Singleton<Interface, Operation> m_interface = Singleton<Interface, Operation>(this);
            OrderedSet<Parameter, Operation> m_operationOwnedParameters = OrderedSet<Parameter, Operation>(this);
            void referenceErased(ID id) override;
            void restoreReferences() override;
            Singleton<Type, Operation>& getTypeSingleton();
            Singleton<Class, Operation>& getClassSingleton();
            Singleton<DataType, Operation>& getDataTypeSingleton();
            Singleton<Interface, Operation>& getInterfaceSingleton();
            Operation();
        public:
            virtual ~Operation();
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
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::OPERATION;
            };
    };
}
