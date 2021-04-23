#ifndef OPERATIONH
#define OPERATIONH

#include "behavioralFeature.h"

namespace UML {

    class Class;

    class Operation : public BehavioralFeature {
        protected:
            Type* m_type;
            Class* m_class;
            void reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) override;
            void reindexName(std::string oldName, std::string newName) override;
        public:
            Operation();
            Type* getType();
            void setType(Type* type);
            Class* getClass();
            void setClass(Class* clazz);
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif