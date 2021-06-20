#ifndef OPERATIONH
#define OPERATIONH

#include "behavioralFeature.h"
#include "templateableElement.h"

namespace UML {

    class Class;
    class DataType;

    class Operation : public BehavioralFeature , public TemplateableElement, public ParameterableElement {
        friend class Parameter;
        protected:
            ID m_typeID;
            Type* m_typePtr;
            ID m_classID;
            Class* m_classPtr;
            ID m_dataTypeID;
            DataType* m_dataTypePtr;
            void reindexID(ID oldID, ID newID) override;
            // void reindexName(std::string oldName, std::string newName) override;
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
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::OPERATION;
            };
    };
}

#endif