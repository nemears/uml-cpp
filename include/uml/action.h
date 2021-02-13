#ifndef ACTION_H
#define ACTION_H

#include "activityNode.h"
#include "inputPin.h"
#include "outputPin.h"

namespace UML {
    class Action : public ActivityNode {
        public:
            list<InputPin*> inputs;
            list<OutputPin*> outputs;
            ElementType getElementType() override;
    };
}

#endif