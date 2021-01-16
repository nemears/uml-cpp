#ifndef SLOT
#define SLOT
#include "element.h"
#include "valueSpecification.h"

namespace UML {
    class Slot : public Element {
        public:
            list<ValueSpecification*> values;
    };
}

#endif