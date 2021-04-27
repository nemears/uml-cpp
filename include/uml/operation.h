#ifndef OPERATIONH
#define OPERATIONH

#include "behavioralFeature.h"

namespace UML {

    class Class;
    class DataType;

    class Operation : public BehavioralFeature {
        friend class Parameter;
        protected:
            Type* m_type;
            Class* m_class;
            DataType* m_dataType;
            void reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) override;
            void reindexName(std::string oldName, std::string newName) override;
        public:
            Operation();
            Type* getType();
            void setType(Type* type);
            Class* getClass();
            void setClass(Class* clazz);
            DataType* getDataType();
            void setDataType(DataType* dataType);
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif