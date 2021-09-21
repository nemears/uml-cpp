#ifndef OPERATIONH
#define OPERATIONH

#include "behavioralFeature.h"
#include "templateableElement.h"
#include "parameterableElement.h"

namespace UML {

    class Class;
    class DataType;
    class Type;
    class Artifact;

    class Operation : public BehavioralFeature , public TemplateableElement, public ParameterableElement {
        
        friend class UmlManager;
        friend class Parameter;

        protected:
            Singleton<Type, Operation> m_type = Singleton<Type, Operation>(this);
            class RemoveTypeProcedure : public AbstractSingletonProcedure<Type, Operation> {
                public:
                    RemoveTypeProcedure(Operation* me) : AbstractSingletonProcedure<Type, Operation>(me) {};
                    void operator()(Type* el) const override;
            };
            class AddTypeProcedure : public AbstractSingletonProcedure<Type, Operation> {
                public:
                    AddTypeProcedure(Operation* me) : AbstractSingletonProcedure<Type, Operation>(me) {};
                    void operator()(Type* el) const override;
            };
            Singleton<Class, Operation> m_class = Singleton<Class, Operation>(this);
            class RemoveClassProcedure : public AbstractSingletonProcedure<Class, Operation> {
                public:
                    RemoveClassProcedure(Operation* me) : AbstractSingletonProcedure<Class, Operation>(me) {};
                    void operator()(Class* el) const override;
            };
            class AddClassProcedure : public AbstractSingletonProcedure<Class, Operation> {
                public:
                    AddClassProcedure(Operation* me) : AbstractSingletonProcedure<Class, Operation>(me) {};
                    void operator()(Class* el) const override;
            };
            Singleton<DataType, Operation> m_dataType = Singleton<DataType, Operation>(this);
            class RemoveDataTypeProcedure : public AbstractSingletonProcedure<DataType, Operation> {
                public:
                    RemoveDataTypeProcedure(Operation* me) : AbstractSingletonProcedure<DataType, Operation>(me) {};
                    void operator()(DataType* el) const override;
            };
            class AddDataTypeProcedure : public AbstractSingletonProcedure<DataType, Operation> {
                public:
                    AddDataTypeProcedure(Operation* me) : AbstractSingletonProcedure<DataType, Operation>(me) {};
                    void operator()(DataType* el) const override;
            };
            Singleton<Artifact, Operation> m_artifact = Singleton<Artifact, Operation>(this);
            class RemoveArtifactProcedure : public AbstractSingletonProcedure<Artifact, Operation> {
                public:
                    RemoveArtifactProcedure(Operation* me) : AbstractSingletonProcedure<Artifact, Operation>(me) {};
                    void operator()(Artifact* el) const override;
            };
            class AddArtifactProcedure : public AbstractSingletonProcedure<Artifact, Operation> {
                public:
                    AddArtifactProcedure(Operation* me) : AbstractSingletonProcedure<Artifact, Operation>(me) {};
                    void operator()(Artifact* el) const override;
            };
            void reindexName(std::string oldName, std::string newName) override;
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            Operation();
        public:
            Operation(const Operation& op);
            virtual ~Operation();
            Type* getType();
            Type& getTypeRef();
            bool hasType() const;
            void setType(Type& type);
            void setType(Type* type);
            Class* getClass();
            Class& getClassRef();
            bool hasClass() const;
            void setClass(Class& clazz);
            void setClass(Class* clazz);
            DataType* getDataType();
            DataType& getDataTypeRef();
            bool hasDataType() const;
            void setDataType(DataType& dataType);
            void setDataType(DataType* dataType);
            Artifact* getArtifact();
            Artifact& getArtifactRef();
            bool hasArtifact() const;
            void setArtifact(Artifact& artifact);
            void setArtifact(Artifact* artifact);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::OPERATION;
            };
    };
}

#endif