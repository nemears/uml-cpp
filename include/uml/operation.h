#ifndef _UML_OPERATION_H_
#define _UML_OPERATION_H_

#include "behavioralFeature.h"
#include "templateableElement.h"
#include "parameterableElement.h"
#include "orderedSet.h"
#include "parameter.h"

namespace UML {

    class Class;
    class DataType;
    class Type;

    typedef UmlPtr<Type> TypePtr;
    typedef UmlPtr<Class> ClassPtr;
    typedef UmlPtr<DataType> DataTypePtr;
    typedef UmlPtr<Interface> InterfacePtr;

    class Operation : public BehavioralFeature , public TemplateableElement, public ParameterableElement {
        
        friend class UmlManager;
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
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(std::string oldName, std::string newName) override;
            void referenceErased(ID id) override;
            void restoreReference(Element* el) override;
            Set<Type, Operation>& getTypeSingleton();
            Set<Class, Operation>& getClassSingleton();
            Set<DataType, Operation>& getDataTypeSingleton();
            Set<Interface, Operation>& getInterfaceSingleton();
            Set<Parameter, Operation>& getOwnedParametersSet();
            void init();
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