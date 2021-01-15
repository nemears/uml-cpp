#ifndef LITERALREAL
#define LITERALREAL
#include "valueSpecification.h"

namespace UML {
    class LiteralReal : public ValueSpecification {
        protected:
            double value;
        public:
            double getValue();
            void setValue(double val);
    };
}

#endif