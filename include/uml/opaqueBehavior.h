#ifndef OPAQUEBEHAVIORH
#define OPAQUEBEHAVIORH

#include <list>
#include <string>
#include "behavior.h"

namespace UML {
    class OpaqueBehavior : public Behavior {
        public:
            list<string> bodies;
    };
}

#endif