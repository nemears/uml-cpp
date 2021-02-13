#ifndef INPUTPIN_H
#define INPUTPIN_H

#include "pin.h"

namespace UML {
    class InputPin : public Pin {
        public:
            ElementType getElementType() override;
    };
}

#endif