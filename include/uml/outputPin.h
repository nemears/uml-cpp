#ifndef OUTPUTPIN_H
#define OUTPUTPIN_H

#include "pin.h"

namespace UML {
    class OutputPin : public Pin {
        public:
            ElementType getElementType() override;
    };
}

#endif