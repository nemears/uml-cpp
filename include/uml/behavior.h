#ifndef BEHAVIORH
#define BEHAVIORH
#include "class.h"
#include "parameter.h"

namespace UML {
    class Behavior : public Class {
        protected:
            Sequence<Parameter>* m_parameters;
        public:
            Behavior();
            ~Behavior();
            Sequence<Parameter>& getParameters();
            ElementType getElementType() override;
    };
}

#endif