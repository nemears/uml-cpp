#ifndef OPERATIONH
#define OPERATIONH

#include "behavioralFeature.h"
#include "templateableElement.h"
#include "parameterableElement.h"
#include "orderedSet.h"

namespace UML {

    class Class;
    class DataType;
    class Type;
    namespace Parsers {
        class OperationSetClass;
        class OperationSetDataType;
        class OperationSetArtifact;
    }

    class Operation : public BehavioralFeature , public TemplateableElement, public ParameterableElement {
        
        friend class UmlManager;
        friend class Class;
        friend class Parameter;
        friend class DataType;
        friend class Parsers::OperationSetClass;
        friend class Parsers::OperationSetDataType;
        friend class Parsers::OperationSetArtifact;

        protected:
            Singleton<Type, Operation> m_type = Singleton<Type, Operation>(this);
            Singleton<Class, Operation> m_class = Singleton<Class, Operation>(this);
            Singleton<DataType, Operation> m_dataType = Singleton<DataType, Operation>(this);
            OrderedSet<Parameter, Operation> m_operationOwnedParameters = OrderedSet<Parameter, Operation>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Set<Type, Operation>& getTypeSingleton();
            Set<Class, Operation>& getClassSingleton();
            Set<DataType, Operation>& getDataTypeSingleton();
            Set<Parameter, Operation>& getOwnedParametersSet();
            void init();
            void copy(const Operation& rhs);
            Operation();
        public:
            Operation(const Operation& rhs);
            virtual ~Operation();
            Type* getType();
            Type& getTypeRef();
            bool hasType() const;
            void setType(Type& type);
            void setType(Type* type);
            Class* getClass();
            Class& getClassRef();
            ID getClassID() const;
            bool hasClass() const;
            void setClass(Class& clazz);
            void setClass(Class* clazz);
            DataType* getDataType();
            DataType& getDataTypeRef();
            ID getDataTypeID() const;
            bool hasDataType() const;
            void setDataType(DataType& dataType);
            void setDataType(DataType* dataType);
            OrderedSet<Parameter, Operation>& getOwnedParameters();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::OPERATION;
            };
    };
}

#endif