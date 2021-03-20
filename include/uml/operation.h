#ifndef OPERATIONH
#define OPERATIONH

#include <list>
#include "namedElement.h"
#include "behavior.h"
#include "parameter.h"


using namespace std;
namespace UML {

    class Class;

    class Operation : public NamedElement {
        protected:
            Type* m_type;
            Sequence<Behavior>* m_methods;
            Class* m_class;
            void reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) override;
            void reindexName(string oldName, string newName) override;
        public:
            Operation();
            ~Operation();
            Sequence<Behavior>& getMethods();
            Type* getType();
            void setType(Type* type);
            Class* getClass();
            void setClass(Class* clazz);
            ElementType getElementType() override;
    };
}

#endif