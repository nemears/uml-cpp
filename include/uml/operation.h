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
        friend class Parameter;
        protected:
            ID m_typeID;
            Type* m_typePtr;
            ID m_classID;
            Class* m_classPtr;
            ID m_dataTypeID;
            DataType* m_dataTypePtr;
            ID m_artifactID;
            Artifact* m_artifactPtr;
            void reindexID(ID oldID, ID newID) override;
            void reindexName(std::string oldName, std::string newName) override;
        public:
            Operation();
            Operation(const Operation& op);
            virtual ~Operation();
            Type* getType();
            void setType(Type* type);
            Class* getClass();
            void setClass(Class* clazz);
            DataType* getDataType();
            void setDataType(DataType* dataType);
            Artifact* getArtifact();
            void setArtifact(Artifact* artifact);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::OPERATION;
            };
    };
}

#endif