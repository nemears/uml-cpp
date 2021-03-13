#ifndef OPAQUEBEHAVIORH
#define OPAQUEBEHAVIORH

#include <list>
#include <string>
#include "behavior.h"

namespace UML {
    class OpaqueBehavior : public Behavior {
        protected:
            
            string singletonBody;
        public:
            list<string> bodies;
            ElementType getElementType() override;
            string getSingletonBody();
            void setSingletonBody(const string& sb);
    };
}

#endif