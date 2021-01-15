#ifndef LITERALINT
#define LITERALINT
#include "valueSpecification.h"

namespace UML {
    class LiteralInt : public ValueSpecification {
        protected:
            int value;
        public:
            int getValue();
            void setValue(int val);
    };
}

#endif