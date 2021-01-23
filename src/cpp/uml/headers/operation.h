#ifndef OPERATION
#define OPERATION

#include <list>
#include "namedElement.h"
#include "behavior.h"
#include "parameter.h"

using namespace std;
namespace UML {
    class Operation : public NamedElement {
        public:
            list<Behavior*> methods;
            list<Parameter*> parameters;
    };
}

#endif