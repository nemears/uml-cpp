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
            Sequence<Parameter>* m_parameters;
            Class* m_class;
        public:
            Operation();
            ~Operation();
            Sequence<Behavior>& getMethods();
            Sequence<Parameter>& getParameters();
            Type* getType();
            void setType(Type* type);
            Class* getClass();
            void setClass(Class* clazz);
            ElementType getElementType() override;
    };
}

#endif