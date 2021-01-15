#ifndef LITERALSTRING
#define LITERALSTRING
#include "valueSpecification.h"
#include <string>

namespace UML {
    class LiteralString : public ValueSpecification {
        protected:
            string value;
        public:
            string getValue();
            void setValue(string val);
    };
}

#endif