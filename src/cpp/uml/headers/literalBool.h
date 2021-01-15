#ifndef LITERALBOOL
#define LITERALBOOL
#include "valueSpecification.h"

namespace UML {
    class LiteralBool : public ValueSpecification {
        protected:
            bool value;
        public:
            bool getValue();
            void setValue(bool val);
    };
}

#endif