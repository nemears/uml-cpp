#ifndef _UML_OPERATION_H_
#define _UML_OPERATION_H_

#include "behavioralFeature.h"
#include "templateableElement.h"
#include "parameterableElement.h"
#include "set/orderedSet.h"
#include "parameter.h"

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
        
        template <typename AccessPolicy, typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class Class;
        friend class Parameter;
        friend class DataType;
        friend class Interface;

        protected:
            CustomSingleton<Type, Operation> m_type = CustomSingleton<Type, Operation>(this);
            CustomSingleton<Class, Operation> m_class = CustomSingleton<Class, Operation>(this);
            CustomSingleton<DataType, Operation> m_dataType = CustomSingleton<DataType, Operation>(this);
            CustomSingleton<Interface, Operation> m_interface = CustomSingleton<Interface, Operation>(this);
            CustomOrderedSet<Parameter, Operation> m_operationOwnedParameters = CustomOrderedSet<Parameter, Operation>(this);
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
            void restoreReference(Element* el) override;
            TypedSet<Type, Operation>& getTypeSingleton();
            TypedSet<Class, Operation>& getClassSingleton();
            TypedSet<DataType, Operation>& getDataTypeSingleton();
            TypedSet<Interface, Operation>& getInterfaceSingleton();
            Operation();
        public:
            virtual ~Operation();
            TypePtr getType() const;
            void setType(Type& type);
            void setType(Type* type);
            void setType(ID id);
            ClassPtr getClass() const;
            void setClass(Class& clazz);
            void setClass(Class* clazz);
            void setClass(ID id);
            DataTypePtr getDataType() const;
            void setDataType(DataType& dataType);
            void setDataType(DataType* dataType);
            void setDataType(ID id);
            InterfacePtr getInterface() const;
            void setInterface(Interface* interface_uml);
            void setInterface(Interface& interface_uml);
            void setInterface(ID id);
            OrderedSet<Parameter, Operation>& getOwnedParameters();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::OPERATION;
            };
    };
}

#endif