#ifndef BEHAVIORH
#define BEHAVIORH
#include "class.h"
#include "parameter.h"

namespace UML {
    class Behavior : public Class {
        public:
            list<Parameter*> parameters;
            ElementType getElementType() override;
    };
}

#endif