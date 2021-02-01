#ifndef OPERATIONH
#define OPERATIONH

#include <list>
#include "namedElement.h"
#include "behavior.h"
#include "parameter.h"

using namespace std;
namespace UML {
    class Operation : public NamedElement {
        protected:
            Type* type;
        public:
            list<Behavior*> methods;
            list<Parameter*> parameters;
            Type* getType();
            void setType(Type* type);
            ElementType getElementType() override;
            Operation() {
                type = NULL;
            }
    };
}

#endif