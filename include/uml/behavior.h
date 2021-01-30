#ifndef BEHAVIORH
#define BEHAVIORH
#include "class.h"

namespace UML {
    class Behavior : public Class {
        ElementType getElementType() override;
    };
}

#endif