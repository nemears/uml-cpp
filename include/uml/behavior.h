#ifndef BEHAVIORH
#define BEHAVIORH
#include "class.h"

namespace UML {
    class Behavior : public Class {
        public:
            ElementType getElementType() override;
    };
}

#endif